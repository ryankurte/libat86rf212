
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>
#include <getopt.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>


enum mode_e {
    MODE_RX,
    MODE_TX
};

struct config_s {
    int mode;
    int channel;
    int help;
    uint16_t address;
    uint16_t pan_id;
};

#include "usbthing/usbthing.h"
#include "usbthing_bindings.h"

#include "at86rf212/at86rf212.hpp"
#include "at86rf212_version.h"

#define DEFAULT_VID     0x0001
#define DEFAULT_PID     0x0001

int parse_args (int argc, char **argv, struct config_s *config);
int print_help (int argc, char **argv);

static volatile int running = 1;

void int_handler(int dummy)
{
    running = 0;
}

void run_rx(AT86RF212::At86rf212* radio)
{
    int res;
    uint8_t len_in;
    uint8_t data_in[128];

    res = radio->start_rx();
    if (res < 0) {
        printf("Error %d starting receive\r\n", res);
    }

    while (running) {
        res = radio->check_rx();

        if (res < 0) {
            printf("Error %d checking receive\r\n", res);
            break;
        } else if (res > 0) {
            printf("Received packet\r\n");

            res = radio->get_rx(&len_in, data_in);
            if (res < 0) {
                printf("Error %d fetching received packet\r\n", res);
            }

            radio->start_rx();
        }
    }
}

int build_header(uint8_t seq, uint16_t pan, uint16_t dest_addr, uint16_t src_addr, uint8_t* packet)
{
    uint8_t header[9];

    header[0] = 0x61;
    header[1] = 0x88;
    header[2] = seq;
    header[3] = pan & 0xFF;
    header[4] = (pan >> 8) & 0xFF;
    header[5] = dest_addr & 0xFF;
    header[6] = (dest_addr >> 8) & 0xFF;
    header[7] = src_addr & 0xFF;
    header[8] = (src_addr >> 8) & 0xFF;

    for (int i = 0; i < sizeof(header); i++) {
        packet[i] = header[i];
    }

    return sizeof(header);
}

int add_data(int len, uint8_t* data_in, uint8_t* packet)
{
    for (int i = 0; i < len; i++) {
        packet[i] = data_in[i];
    }
    return len;
}

//0b 61 88 05 00 01 01 01 02 02 80 01
void run_tx(AT86RF212::At86rf212* radio)
{
    uint8_t data_read[127];
    uint8_t data_out[127];
    uint8_t len_read = 0;
    uint8_t len_out = 0;
    int res;
    uint8_t seq = 0;

    printf("Interactive transmit mode, type 'exit' to exit\r\n");

    while (running) {
        char* line = readline(">");

        if (line != NULL) {
            add_history(line);

            if (strncmp(line, "exit", 4) == 0) {
                break;
            }

            char* token = strtok (line, " ,");
            while (token != NULL) {
                data_read[len_read] = (uint8_t)strtol(token, NULL, 16);
                len_read ++;
                token = strtok (NULL, " ,");
            }

            if (len_read > 0) {

                // TODO: build header only if configured
                len_out = build_header(seq, 0x0100, 0x0000, 0x0001, data_out);
                for (int i = 0; i < len_read; i++) {
                    data_out[len_out + i] = data_read[i];
                }
                len_out += len_read;

                seq += 1;

                printf("Output: ");
                for (int i = 0; i < len_out; i++) {
                    printf("%.2x ", data_out[i]);
                }
                printf("\r\n");

                res = radio->start_tx(len_out, data_out);
                if (res < 0) {
                    printf("Error %d starting send\r\n", res);
                    break;
                }

                while (1) {
                    res = radio->check_tx();
                    if (res < 0) {
                        printf("Error %d checking send\r\n", res);
                        break;
                    } else if (res > 0) {
                        printf("Sent %d bytes\r\n", len_out);
                        break;
                    }
                }
            }

            free(line);
            len_read = 0;
            len_out = 0;
        }
    }
}

int main(int argc, char** argv)
{
    int res;
    struct config_s config;

    // at86rf212 driver object
    struct at86rf212_driver_s at86rf212_driver;
    at86rf212_driver.spi_transfer = spi_transfer;
    at86rf212_driver.set_reset = set_reset;
    at86rf212_driver.set_slp_tr = set_slp_tr;
    at86rf212_driver.get_irq = get_irq;

    usbthing_t usbthing;
    AT86RF212::At86rf212 radio = AT86RF212::At86rf212();

    char version[32];

    // Parse command line arguments
    res = parse_args(argc, argv, &config);
    if (res < 0) {
        return 0;
    }

    // Ensure mode is valid
    if (config.mode < 0) {
        printf("--mode argument is required\r\n");
        print_help(argc, argv);
        return 0;
    }


    // Initialise USB-Thing static components
    USBTHING_init();

    // Connect to USB-Thing device
    res = USBTHING_connect(&usbthing, DEFAULT_VID, DEFAULT_PID);
    if (res < 0) {
        printf("Error %d opening USB-Thing\r\n", res);
        goto end;
    }

    // Configure SPI speed and mode
    res = USBTHING_spi_configure(usbthing, 400000, 0);
    if (res < 0) {
        printf("Error %d setting SPI speed\r\n", res);
        goto end;
    }

    USBTHING_gpio_configure(usbthing, 0, 1, 0, 0);
    USBTHING_gpio_configure(usbthing, 1, 1, 0, 0);
    USBTHING_gpio_configure(usbthing, 2, 0, 0, 0);


    // Connect to and configure radio
    res = radio.init(&at86rf212_driver, (void*) &usbthing);
    if (res < 0) {
        printf("Error %d initialising AT86RF212\r\n", res);
        goto end;
    }

    if (config.address != 0) {
        printf ("address: %d\r\n", config.address);
        res = radio.set_short_address(config.address);
        if (res < 0) {
            printf("Error %d setting address\r\n", res);
            goto end;
        }
    }

    if (config.pan_id != 0) {
        printf ("pan id: %d\r\n", config.pan_id);
        res = radio.set_pan_id(config.pan_id);
        if (res < 0) {
            printf("Error %d setting pan_id\r\n", res);
            goto end;
        }
    }

    res = radio.set_channel(config.channel);
    if (res < 0) {
        printf("Error %d setting channel\r\n", res);
        goto end;
    }


    // Bind signal handler to exit
    signal(SIGINT, int_handler);

    // Run mode specific main loops
    switch (config.mode) {
    case MODE_RX:
        run_rx(&radio);
        break;
    case MODE_TX:
        run_tx(&radio);
        break;
    }

end:
    printf("Exiting\r\n");

    radio.close();

    // Disconnect from the USB-Thing
    res = USBTHING_disconnect(&usbthing);
    if (res < 0) {
        printf("Error %d closing USB-Thing\r\n", res);
        return -2;
    }

    // Close USB-Thing static components
    USBTHING_close();

    return 0;
}

int print_help (int argc, char **argv)
{
    printf("at86rf212b-util (%s)\r\n", LIBAT86RF212_VERSION_STRING);
    printf("Usage: %s --mode=[rx|tx] [--channel=N --verbose]\r\n", argv[0]);

    printf("\r\n");
    return 0;
}

int parse_args (int argc, char **argv, struct config_s *config)
{
    int c;

    // Set defaults
    config->mode = -1;
    config->channel = 1;
    config->pan_id = 0;
    config->address = 0;

    static struct option long_options[] = {
        /* These options set a flag. */
        {"mode",    required_argument,  0,              'm'},
        {"channel", required_argument,  0,              'c'},
        {"address", required_argument,  0,              'a'},
        {"pan",     required_argument,  0,              'p'},
        {"help",    no_argument,        0,              'h'},
        {"version", no_argument,        0,              'v'},
        {0,         0,                  0,              0}
    };
    /* getopt_long stores the option index here. */
    int option_index = 0;

    while (1) {

        c = getopt_long (argc, argv, "vm:c:h",
                         long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c) {
        case 'c':
            config->channel = atoi(optarg);
            printf ("channel: %d\r\n", config->channel);
            break;

        case 'm':
            if (strncmp (optarg, "rx", 2) == 0) {
                config->mode = MODE_RX;
                printf("receive mode\r\n");
            } else if (strncmp (optarg, "tx", 2) == 0) {
                config->mode = MODE_TX;
                printf("transmit mode\r\n");
            } else {
                printf("Unrecognised mode: %s\r\n", optarg);
                return -1;
            }
            break;

        case 'a':
            config->address = atoi(optarg);
            break;

        case 'p':
            config->pan_id = atoi(optarg);
            break;

        case 'v':
            printf("%s\r\n", LIBAT86RF212_VERSION_STRING);
            return -1;

        case 'h':
        case '?':
            print_help(argc, argv);
            return -1;
            break;

        default:
            abort();
        }
    }

    return 0;
}
