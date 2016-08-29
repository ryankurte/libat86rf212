
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>
#include <getopt.h>
#include <string.h>

enum mode_e {
    MODE_RX,
    MODE_TX
};

struct config_s {
    int verbose;
    int mode;
    int channel;
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
    char version[32];

    parse_args(argc, argv, &config);
    if (config.mode < 0) {
        printf("--mode argument is required\r\n");
        print_help(argc, argv);
        return 0;
    }

    // Create the MPU device
    AT86RF212::At86rf212 radio = AT86RF212::At86rf212();

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

    printf("Connected to USB-Thing\r\n");

    // Connect to and configure MPU
    res = radio.init(&at86rf212_driver, (void*) &usbthing);
    if (res < 0) {
        printf("Error %d initialising AT86RF212\r\n", res);
        goto end;
    }

    printf("Radio initialised\r\n");

    // Bind signal handler to exit
    signal(SIGINT, int_handler);

    res = radio.set_channel(config.channel);
    if (res < 0) {
        printf("Error %d setting channel\r\n", res);
        goto end;
    }

    uint8_t len_in;
    uint8_t data_in[128];

    switch (config.mode) {
    case MODE_RX:
        res = radio.start_rx();
        if (res < 0) {
            printf("Error %d starting receive\r\n", res);
        }

        while (running) {
            res = radio.check_rx();

            if (res < 0) {
                printf("Error %d checking receive\r\n", res);
                break;
            } else if (res > 0) {
                printf("Received packet\r\n");

                res = radio.get_rx(&len_in, data_in);
                if (res < 0) {
                    printf("Error %d fetching received packet\r\n", res);
                }

                radio.start_rx();
            }
        }

        break;
    case MODE_TX:

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
    printf("AT86RF212B Util\r\n");
    printf("AT86RF212B Library Version: %s\r\n", LIBAT86RF212_VERSION_STRING);

    printf("Usage:\r\n");
    printf("%s --mode=[rx|tx] --channel=N\r\n", argv[0]);

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
        {"verbose", no_argument,        &config->verbose,  1},
        {"mode",    required_argument,  0,              'm'},
        {"channel", required_argument,  0,              'c'},
        {"address", required_argument,  0,              'a'},
        {"pan",     required_argument,  0,              'p'},
        {"help",    no_argument,        0,              'h'},
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

        case 'h':
        case '?':
            print_help(argc, argv);
            break;

        default:
            abort();
        }
    }

    return 0;
}
