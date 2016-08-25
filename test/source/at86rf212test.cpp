

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <stdint.h>

#include "at86rf212/at86rf212.hpp"
#include "mock_spi.hpp"
#include "usbthing/usbthing.h"
#include "usbthing_bindings.h"

using namespace AT86RF212;
using namespace testing;

#define DEFAULT_VID     0x0001
#define DEFAULT_PID     0x0001


// The fixture for testing class Foo.
class At86rf212Test : public ::testing::Test
{
protected:

  void SetUp()
  {
    int res;

    at86rf212_driver.spi_transfer = spi_transfer;
    at86rf212_driver.set_reset = set_reset;
    at86rf212_driver.set_slp_tr = set_slp_tr;
    at86rf212_driver.get_irq = get_irq;

    USBTHING_init();

    // Connect to USB-Thing device
    res = USBTHING_connect(&usbthing, DEFAULT_VID, DEFAULT_PID);
    EXPECT_EQ(0, res);

    // Configure SPI speed and mode
    res = USBTHING_spi_configure(usbthing, 400000, 0);
    ASSERT_TRUE(res >= 0);

    USBTHING_gpio_configure(usbthing, 0, 1, 0, 0);
    USBTHING_gpio_configure(usbthing, 1, 1, 0, 0);
    USBTHING_gpio_configure(usbthing, 2, 0, 0, 0);

    radio = At86rf212();

    res = radio.init(&at86rf212_driver, (void*) &usbthing);
    EXPECT_EQ(0, res);

  }

  void TearDown()
  {
    int res;

    radio.close();

    // Disconnect from the USB-Thing
    res = USBTHING_disconnect(&usbthing);
    EXPECT_EQ(0, res);

    // Close USB-Thing static components
    USBTHING_close();

  }

  usbthing_t usbthing;
  struct at86rf212_driver_s at86rf212_driver;
  At86rf212 radio;
};

TEST_F(At86rf212Test, SetState)
{
  int res;
  uint8_t state;

  res = radio.set_state(AT86RF212_CMD_RX_ON);
  ASSERT_TRUE(res >= 0);

  res = radio.get_state(&state);
  ASSERT_TRUE(res >= 0);
  EXPECT_EQ(AT86RF212_CMD_RX_ON, state);

  res = radio.set_state(AT86RF212_CMD_TRX_OFF);
  ASSERT_TRUE(res >= 0);

  res = radio.get_state(&state);
  ASSERT_TRUE(res >= 0);
  EXPECT_EQ(AT86RF212_CMD_TRX_OFF, state);
}

TEST_F(At86rf212Test, SetChannel)
{
  int res;
  uint8_t channel = 0;

  res = radio.set_channel(10);
  ASSERT_TRUE(res >= 0);

  res = radio.get_channel(&channel);
  ASSERT_TRUE(res >= 0);
  EXPECT_EQ(10, channel);

  res = radio.set_channel(1);
  ASSERT_TRUE(res >= 0);

  res = radio.get_channel(&channel);
  ASSERT_TRUE(res >= 0);
  EXPECT_EQ(1, channel);
}



