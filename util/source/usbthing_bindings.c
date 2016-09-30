
#include "usbthing_bindings.h"

#include <stdint.h>

#include "usbthing/usbthing.h"

//#define DEBUG_INTERCEPT_SPI

#ifdef DEBUG_INTERCEPT_SPI
#include <stdio.h>
#endif

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

int set_reset(void* context, uint8_t val)
{
    usbthing_t *usbthing = (usbthing_t *) context;

    int res = USBTHING_gpio_set(*usbthing, 0, val);

    return res;
}

int set_slp_tr(void* context, uint8_t val)
{
    usbthing_t *usbthing = (usbthing_t *) context;

    int res = USBTHING_gpio_set(*usbthing, 1, val);

    return res;
}

int get_irq(void* context, uint8_t *val)
{
    usbthing_t *usbthing = (usbthing_t *) context;
    int val_int;

    int res = USBTHING_gpio_get(*usbthing, 2, &val_int);

    *val = val_int;

    return res;
}

