
#include "usbthing_bindings.h"

#include <stdint.h>

#include "usbthing/usbthing.h"

// SPI transfer binding
int spi_transfer(void* context, int len, uint8_t *data_out, uint8_t* data_in)
{
    usbthing_t *usbthing = (usbthing_t *) context;
    int res;

    res = USBTHING_spi_transfer(*usbthing, len, data_out, data_in);

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

