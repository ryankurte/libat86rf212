

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <stdint.h>
#include <time.h>

#include "at86rf212/at86rf212.hpp"
#include "at86rf212/at86rf212_regs.h"
#include "at86rf212/at86rf212_defs.h"

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
    ASSERT_EQ(0, res);

    // Close USB-Thing static components
    USBTHING_close();

  }

  usbthing_t usbthing;
  struct at86rf212_driver_s at86rf212_driver;
  At86rf212 radio;
};

TEST_F(At86rf212Test, InitialConfig)
{
  int res;
  uint8_t val;

  // Check default CCA mode
  res = radio.read_reg(AT86RF212_REG_PHY_CC_CCA, &val);
  ASSERT_EQ(0, res);
  EXPECT_EQ(1, (val >> 5) & 0x03);

  // Check default CSMA exponents
  res = radio.read_reg(AT86RF212_REG_CSMA_BE, &val);
  ASSERT_EQ(0, res);
  EXPECT_EQ(AT86RF212_DEFAULT_MINBE, (val >> 0) & 0x0f);
  EXPECT_EQ(AT86RF212_DEFAULT_MAXBE, (val >> 4) & 0x0f);

  // Check default CSMA backoff
  res = radio.read_reg(AT86RF212_REG_XAH_CTRL_0, &val);
  ASSERT_EQ(0, res);
  EXPECT_EQ(AT86RF212_DEFAULT_MAX_CSMA_BACKOFFS, (val >> 1) & 0x07);

  // Check radio is in promiscuous mode
  res = radio.read_reg(AT86RF212_REG_XAH_CTRL_1, &val);
  ASSERT_EQ(0, res);
  EXPECT_EQ(1, (val >> 1) & 0x01);

  // Check auto CRC is enabled
  res = radio.read_reg(AT86RF212_REG_TRX_CTRL_1, &val);
  ASSERT_EQ(0, res);
  EXPECT_EQ(1, (val >> 5) & 0x01);

  // Check IRQ_MASK_MODE is set
  res = radio.read_reg(AT86RF212_REG_TRX_CTRL_1, &val);
  ASSERT_EQ(0, res);
  EXPECT_EQ(1, (val >> 1) & 0x01);

}

TEST_F(At86rf212Test, SetState)
{
  int res;
  uint8_t state;

  res = radio.set_state(AT86RF212_CMD_RX_ON);
  ASSERT_TRUE(res >= 0);

  res = radio.get_state(&state);
  ASSERT_TRUE(res >= 0);
  ASSERT_EQ(AT86RF212_CMD_RX_ON, state);

  res = radio.set_state(AT86RF212_CMD_TRX_OFF);
  ASSERT_TRUE(res >= 0);

  res = radio.get_state(&state);
  ASSERT_TRUE(res >= 0);
  ASSERT_EQ(AT86RF212_CMD_TRX_OFF, state);
}

TEST_F(At86rf212Test, SetChannel)
{
  int res;
  uint8_t channel = 0;

  res = radio.set_channel(10);
  ASSERT_TRUE(res >= 0);

  res = radio.get_channel(&channel);
  ASSERT_TRUE(res >= 0);
  ASSERT_EQ(10, channel);

  res = radio.set_channel(1);
  ASSERT_TRUE(res >= 0);

  res = radio.get_channel(&channel);
  ASSERT_TRUE(res >= 0);
  ASSERT_EQ(1, channel);
}


#define HEADER_SIZE       (2+1+(2+2+2)+0)
#define SEND_TIMEOUT_S    2
uint8_t radio_header[HEADER_SIZE] = {0x61, 0x88, 0x05, 0x00, 0x01, 0x01, 0X01, 0x02, 0x02};
#define DATA_SIZE         4
uint8_t radio_data[DATA_SIZE] = {0x80, 0x01, 'A', 'L'};

TEST_F(At86rf212Test, Send)
{
  int res;
  uint8_t channel = 0;
  uint8_t data[HEADER_SIZE + DATA_SIZE + 2 + 1];

  data[0] = HEADER_SIZE + DATA_SIZE + 2;

  for (int i = 0; i < HEADER_SIZE; i++) {
    data[i + 1] = radio_header[i];
  }
  for (int i = 0; i < DATA_SIZE; i++) {
    data[i + HEADER_SIZE + 1] = radio_data[i];
  }
  for (int i = 0; i < 2; i++) {
    data[i + HEADER_SIZE + DATA_SIZE + 1] = 0x00;
  }

  res = radio.start_tx(sizeof(data), data);
  ASSERT_EQ(AT86RF212_RES_OK, res);

  int start_time = time(NULL);

  while (1) {
    res = radio.check_tx();
    if (res == 1) {
      break;
    }
    if (time(NULL) > (start_time + 1)) {
      ASSERT_TRUE(false);
    }
  }
}

