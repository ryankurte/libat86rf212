
#ifndef USBTHING_BINDINGS_H
#define USBTHING_BINDINGS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int spi_transfer(void* context, int len, uint8_t *data_out, uint8_t* data_in);
int set_reset(void* context, uint8_t val);
int set_slp_tr(void* context, uint8_t val);
int get_irq(void* context, uint8_t *val);

#ifdef __cplusplus
}
#endif

#endif
