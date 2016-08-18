
#pragma once

#include "gmock/gmock.h"

#include <stdint.h>

#include "at86rf212/at86rf212_if.hpp"

class MockSpi : public AT86RF212::SpiDriverInterface
{
public:
    MOCK_METHOD3(spi_transfer, int(int len, uint8_t* data_out, uint8_t *data_in));

};