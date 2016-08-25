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

private:
    struct at86rf212_s device;
};


};

