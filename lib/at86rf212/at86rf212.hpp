/*
 * at86rf212 c++ wrapper
 *
 * Copyright 2016 Ryan Kurte
 */

#pragma once

#include "at86rf212_if.hpp"

namespace AT86RF212
{

#include "at86rf212.h"
#include "at86rf212_defs.h"

class At86rf212
{
public:

    // Init using C style driver interface
    int init(struct at86rf212_driver_s *driver, void *driver_ctx)
    {
        return at86rf212_init(&(this->device), driver, driver_ctx);
    }

    // Init using C++ style driver interface
    int init(SpiDriverInterface* driver_ctx)
    {
        struct at86rf212_driver_s *driver = SpiDriverWrapper::GetWrapper();
        return at86rf212_init(&(this->device), driver, (void*)driver_ctx);
    }

    // Close device
    int close()
    {
        return at86rf212_close(&(this->device));
    }
    int set_short_address(uint16_t address)
    {
        return at86rf212_set_short_address(&(this->device), address);
    }
    int set_pan_id(uint16_t pan_id)
    {
        return at86rf212_set_pan_id(&(this->device), pan_id);
    }

    int set_state(uint8_t state)
    {
        return at86rf212_set_state(&(this->device), state);
    }
    int set_state_blocking(uint8_t state)
    {
        return at86rf212_set_state_blocking(&(this->device), state);
    }
    int get_state(uint8_t *state)
    {
        return at86rf212_get_state(&(this->device), state);
    }
    int set_channel(uint8_t channel)
    {
        return at86rf212_set_channel(&(this->device), channel);
    }
    int get_channel(uint8_t *channel)
    {
        return at86rf212_get_channel(&(this->device), channel);
    }
    int start_tx(uint8_t length, uint8_t* data)
    {
        return at86rf212_start_tx(&(this->device), length, data);
    }
    int check_tx()
    {
        return at86rf212_check_tx(&(this->device));
    }

    int start_rx()
    {
        return at86rf212_start_rx(&(this->device));
    }
    int check_rx()
    {
        return at86rf212_check_rx(&(this->device));
    }
    int get_rx(uint8_t* length, uint8_t* data)
    {
        return at86rf212_get_rx(&(this->device), length, data);
    }

    int read_reg(uint8_t reg, uint8_t* val)
    {
        return at86rf212_read_reg(&(this->device), reg, val);
    }
    int write_reg(uint8_t reg, uint8_t val)
    {
        return at86rf212_write_reg(&(this->device), reg, val);
    }

private:
    struct at86rf212_s device;
};


};

