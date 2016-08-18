/*
 * at86rf212 driver functions
 *
 * Copyright 2016 Ryan Kurte
 */

#include "at86rf212/at86rf212.h"

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "at86rf212/at86rf212_regs.h"

//#define DEBUG_AT86RF212


// Automagically define PLATFORM_SLEEP_MS on unix-like platforms
#ifndef PLATFORM_SLEEP_MS
#if (defined __linux__ || defined __APPLE__ || defined __unix__)
#include <unistd.h>
#define PLATFORM_SLEEP_MS(a)    usleep(a * 1000);
#else
#error "PLATFORM_SLEEP_MS undefined and platform not recognised"
#endif

#endif

// Wrap debug outputs
#ifdef DEBUG_AT86RF212
#define AT86RF212_DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define AT86RF212_DEBUG_PRINT(...)
#endif


/***        Internal Functions          ***/

// Read a single register from the device
static int at86rf212_read_reg(struct at86rf212_s *device, uint8_t reg, uint8_t* val)
{
    uint8_t data_out[2] = {0xFF, 0xFF};
    uint8_t data_in[2] = {0xFF, 0xFF};
    int res;

    data_out[0] = reg | AT86RF212_REG_READ_FLAG;
    data_out[1] = 0x00;

    res = device->driver->spi_transfer(device->driver_ctx, 2, data_out, data_in);

    if (res >= 0) {
        *val = data_in[1];
    }

    return res;
}

// Read an array of registers from the device
static int at86rf212_read_regs(struct at86rf212_s *device, uint8_t start, uint8_t length, uint8_t* data)
{
    uint8_t data_out[length + 1];
    uint8_t data_in[length + 1];
    int res;

    data_out[0] = start | AT86RF212_REG_READ_FLAG;
    for (int i = 0; i < length; i++) {
        data_out[i + 1] = 0x00;
    }

    res = device->driver->spi_transfer(device->driver_ctx, length + 1, data_out, data_in);

    if (res >= 0) {
        for (int i = 0; i < length; i++) {
            data[i] = data_in[i + 1];
        }
    }

    return res;
}

// Write a single register on the device
static int at86rf212_write_reg(struct at86rf212_s *device, uint8_t reg, uint8_t val)
{
    uint8_t data_out[2] = {0xFF, 0xFF};
    uint8_t data_in[2] = {0xFF, 0xFF};
    int res;

    data_out[0] = reg | AT86RF212_REG_WRITE_FLAG;
    data_out[1] = val;

    res = device->driver->spi_transfer(device->driver_ctx, 2, data_out, data_in);

    return res;
}

// Update a single register on the device
// Mask should be the mask for the information to be changed
int at86rf212_update_reg(struct at86rf212_s *device, uint8_t reg, uint8_t val, uint8_t mask)
{
    uint8_t data = 0;
    int res;

    // Read existing config
    res = at86rf212_read_reg(device, reg, &data);
    if (res < 0) {
        return res;
    }

    // Update
    data &= ~mask;
    data |=  mask & val;

    // Write back
    return at86rf212_write_reg(device, reg, data);
}


/***        External Functions          ***/

int8_t at86rf212_init(struct at86rf212_s *device, struct at86rf212_driver_s *driver, void* driver_ctx)
{
    int res;

    // Check driver functions exist
    if (driver->spi_transfer == NULL) {
        return AT86RF212_DRIVER_INVALID;
    }

    if (driver->get_irq == NULL) {
        return AT86RF212_DRIVER_INVALID;
    }

    // Save driver pointers
    device->driver = driver;
    device->driver_ctx = driver_ctx;

    // TODO: Initialize device


    // Give device time to reset
    PLATFORM_SLEEP_MS(10);

    AT86RF212_DEBUG_PRINT("RESET complete\r\n");

    // TODO: Check communication


    AT86RF212_DEBUG_PRINT("Device identified\r\n");

    // TODO: Configure


    return 0;
}

int8_t at86rf212_close(struct at86rf212_s *device)
{
    // TODO: shutdown

    // Clear driver pointer
    device->driver = NULL;
    device->driver_ctx = NULL;

    return 0;
}


