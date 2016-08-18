
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>


#include "usbthing/usbthing.h"
#include "at86rf212/at86rf212.hpp"

#define DEFAULT_VID     0x0001
#define DEFAULT_PID     0x0001

static volatile int running = 1;

void int_handler(int dummy)
{
    running = 0;
}

// SPI transfer binding
int spi_transfer(void* context, int len, uint8_t *data_out, uint8_t* data_in)
{
    usbthing_t *usbthing = (usbthing_t *) context;
    int res;

    res = USBTHING_spi_transfer(*usbthing, len, data_out, data_in);

#ifdef DEBUG_INTERCEPT_SPI
    // Intercept and print SPI commands for debugging
    uint8_t reg = data_out[0] & 0x7F;
    uint8_t is_write = ((data_out[0] & 0x80) == 0) ? 1 : 0;

    printf("SPI (reg: 0x%x write: %d) MOSI: ", data_out[0] & 0x7F, is_write);
    for (int i = 0; i < len; i++) {
        printf("%.2x ", data_out[i]);
    }
    printf("MISO: ");
    for (int i = 0; i < len; i++) {
        printf("%.2x ", data_in[i]);
    }
    printf("\r\n");
#endif

    return res;
}

// at86rf212 driver object
struct at86rf212_driver_s at86rf212_driver = { spi_transfer };

int main(int argc, char** argv)
{
    int res;

    usbthing_t usbthing;
    char version[32];

    // Create the MPU device
    AT86RF212::Mpu9250 radio = AT86RF212::At86rf212();

    // Initialise USB-Thing static components
    USBTHING_init();

    // Connect to USB-Thing device
    res = USBTHING_connect(&usbthing, DEFAULT_VID, DEFAULT_PID);
    if (res < 0) {
        printf("Error %d opening USB-Thing\r\n", res);
        goto end;
    }

    // Configure SPI speed and mode
    res = USBTHING_spi_configure(usbthing, 400000, 3);
    if (res < 0) {
        printf("Error %d setting SPI speed\r\n", res);
        goto end;
    }

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

    while (running) {

        //TODO

        usleep(250 * 1000);
    }

end:

    printf("Exiting\r\n");

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
