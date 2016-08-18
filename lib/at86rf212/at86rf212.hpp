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
    int init(struct at86rf212_driver_s *driver, void *driver_ctx) {
        return at86rf212_init(&(this->device), driver, driver_ctx);
    }

    // Init using C++ style driver interface
    int init(SpiDriverInterface* driver_ctx) {
        struct at86rf212_driver_s *driver = SpiDriverWrapper::GetWrapper();
        return at86rf212_init(&(this->device), driver, (void*)driver_ctx);
    }

    // Close device
    int close() {
        return at86rf212_close(&(this->device));
    }

private:
    struct at86rf212_s device;
};


};

