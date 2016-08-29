
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

void run_tx(AT86RF212::At86rf212* radio)
{
    uint8_t data_out[127];
    uint8_t len_out = 0;
    int res;

    printf("Interactive transmit mode, type 'exit' to exit\r\n");

    while (running) {
        char* line = readline(">");

        if (line != NULL) {
            if (strncmp(line, "exit", 4) == 0) {
                break;
            }

            char* token = strtok (line, " ,");
            while (token != NULL) {
                data_out[len_out] = (uint8_t)strtol(token, NULL, 16);
                len_out ++;
                token = strtok (NULL, " ,");
            }

            if (len_out > 0) {
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
                        printf("Send complete\r\n");
                        break;
                    }
                }
            }

            add_history(line);
            free(line);
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

    res = radio.set_short_address(config.address);
    if (res < 0) {
        printf("Error %d setting address\r\n", res);
        goto end;
    }

    res = radio.set_pan_id(config.pan_id);
    if (res < 0) {
        printf("Error %d setting pan_id\r\n", res);
        goto end;
    }

    res = radio.set_channel(config.channel);
    if (res < 0) {
        printf("Error %d setting channel\r\n", res);
        goto end;
    }


    // Bind signal handler to exit
    signal(SIGINT, int_handler);




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
    config->pan_id = 1;
    config->address = 1;

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
            printf ("address: %d\r\n", config->address);
            break;

        case 'p':
            config->pan_id = atoi(optarg);
            printf ("pan_id: %d\r\n", config->pan_id);
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
