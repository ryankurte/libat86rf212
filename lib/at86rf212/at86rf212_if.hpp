/*
 * at86rf212 c++ to c driver adapter
 * Converts between a c++ driver classes and underlying c function calls.
 *
 * Copyright 2016 Ryan Kurte
 */

#pragma once

#include "at86rf212.h"

namespace AT86RF212
{

// SPI driver interface class
class At86rf212DriverInterface
{
public:
    virtual int spi_transfer(int len, uint8_t *data_out, uint8_t* data_in) = 0;
    virtual int set_sdn(uint8_t val) = 0;
    virtual int set_slp_tr(uint8_t val) = 0;
    virtual int get_irq(uint8_t *val) = 0;
};

// Adaptor functions, allows c++ object to be called from c(ish) context
int at86rf212_transfer_data_adaptor(void* context, int len, uint8_t* data_out, uint8_t* data_in)
{
    At86rf212DriverInterface *driver = (At86rf212DriverInterface*) context;
    return driver->spi_transfer(len, data_out, data_in);
}

int at86rf212_set_sdn_adaptor(void* context, uint8_t val)
{
    At86rf212DriverInterface *driver = (At86rf212DriverInterface*) context;
    return driver->set_sdn(val);
}

int at86rf212_set_slp_tr_adaptor(void* context, uint8_t val)
{
    At86rf212DriverInterface *driver = (At86rf212DriverInterface*) context;
    return driver->set_slp_tr(val);
}

int at86rf212_get_irq_adaptor(void* context, uint8_t* val)
{
    At86rf212DriverInterface *driver = (At86rf212DriverInterface*) context;
    return driver->get_irq(val);
}

// SPI Driver wrapper object
// Adapts C++ driver object for use in C based library
// Note that this can be static as driver context is passed separately to the driver
class SpiDriverWrapper
{
public:
    static struct at86rf212_driver_s* GetWrapper()
    {
        return &driver;
    }

private:
    static struct at86rf212_driver_s driver;
};

struct at86rf212_driver_s SpiDriverWrapper::driver = {
    at86rf212_transfer_data_adaptor,
    at86rf212_set_sdn_adaptor,
    at86rf212_set_slp_tr_adaptor,
    at86rf212_get_irq_adaptor
};


};

