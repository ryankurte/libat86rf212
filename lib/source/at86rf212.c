/*
 * at86rf212 driver functions
 *
 * Copyright 2016 Ryan Kurte
 */

#include "at86rf212/at86rf212.h"

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#include "at86rf212/at86rf212_regs.h"

//#define DEBUG_AT86RF212

// Automagically define PLATFORM_SLEEP_MS and _US on unix-like platforms
#ifndef PLATFORM_SLEEP_MS
#if (defined __linux__ || defined __APPLE__ || defined __unix__)
#include <unistd.h>
#define PLATFORM_SLEEP_MS(a)    usleep(a * 1000);
#define PLATFORM_SLEEP_US(a)    usleep(a);
#else
#warning "PLATFORM_SLEEP_MS undefined and platform not recognised"
#define PLATFORM_SLEEP_MS(a) for (volatile uint32_t i = 0; i < 1000000; i++);
#define PLATFORM_SLEEP_US(a) for (volatile uint32_t i = 0; i < 1000; i++);
#endif

#else
extern void PLATFORM_SLEEP_MS(uint32_t);
extern void PLATFORM_SLEEP_US(uint32_t);
#endif

// Wrap debug outputs
#ifdef DEBUG_AT86RF212
#include <stdio.h>
#define AT86RF212_DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define AT86RF212_DEBUG_PRINT(...)
#endif

#define AT86RF212_MAX_RETRIES       1000


/***        Internal Functions          ***/

// Read a single register from the device
int at86rf212_read_reg(struct at86rf212_s *device, uint8_t reg, uint8_t* val)
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


// Write a single register on the device
int at86rf212_write_reg(struct at86rf212_s *device, uint8_t reg, uint8_t val)
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
int at86rf212_update_reg(struct at86rf212_s *device, uint8_t reg, uint8_t mask, uint8_t val)
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

// Write a subregister on the device
// Implemented for compatibility with atmel supplied subregister headers (if you want to use those)
int at86rf212_write_subreg(struct at86rf212_s *device, uint8_t reg, uint8_t mask, uint8_t shift, uint8_t val)
{
    return at86rf212_update_reg(device, reg, mask, val << shift);
}

// Read a frame from the device
static int at86rf212_read_frame(struct at86rf212_s *device, uint8_t length, uint8_t* data)
{
    uint8_t data_out[length + 1];
    uint8_t data_in[length + 1];
    int res;

    data_out[0] = AT86RF212_FRAME_READ_FLAG;
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

// Write a frame to the device
static int at86rf212_write_frame(struct at86rf212_s *device, uint8_t length, uint8_t* data)
{
    uint8_t data_out[length + 1];
    uint8_t data_in[length + 1];

    data_out[0] = AT86RF212_FRAME_WRITE_FLAG;
    for (int i = 0; i < length; i++) {
        data_out[i + 1] = data[i];
    }

    return device->driver->spi_transfer(device->driver_ctx, length + 1, data_out, data_in);
}

/***        External Functions          ***/

int at86rf212_init(struct at86rf212_s *device, struct at86rf212_driver_s *driver, void* driver_ctx)
{
    int res;
    uint8_t val;

    // Check driver functions exist
    if (driver->spi_transfer == NULL) {
        return AT86RF212_DRIVER_INVALID;
    }

    if ((driver->get_irq == NULL) || (driver->set_reset == NULL)
        || (driver->set_slp_tr == NULL)) {
        return AT86RF212_DRIVER_INVALID;
    }

    // Save driver pointers
    device->driver = driver;
    device->driver_ctx = driver_ctx;

    // Initialize device

    // Set pins
    device->driver->set_reset(device->driver_ctx, 1);
    device->driver->set_slp_tr(device->driver_ctx, 0);

    // Send reset pulse
    device->driver->set_reset(device->driver_ctx, 0);
    PLATFORM_SLEEP_MS(1);
    device->driver->set_reset(device->driver_ctx, 1);

    // Give device time to reset
    PLATFORM_SLEEP_MS(10);

    //AT86RF212_DEBUG_PRINT("RESET complete\r\n");

    // Check communication
    uint8_t who;
    res = at86rf212_read_reg(device, AT86RF212_REG_PART_NUM, &who);
    if (res < 0) {
        AT86RF212_DEBUG_PRINT("WHOAMI read error: %d\r\n", res);
        return AT86RF212_ERROR_DRIVER;
    }
    if (who != 0x07) {
        AT86RF212_DEBUG_PRINT("Unexpected whoami response: %.2x\r\n", who);
        return AT86RF212_ERROR_COMMS;
    }

    //AT86RF212_DEBUG_PRINT("Device identified\r\n");

    // TODO: Configure

    // Disable TRX
    res = at86rf212_set_state_blocking(device, AT86RF212_CMD_TRX_OFF);
    if (res < 0) {
        AT86RF212_DEBUG_PRINT("Mode set error: %d\r\n", res);
        return AT86RF212_ERROR_DRIVER;
    }

    // Check Digital Voltage
    res = at86rf212_read_reg(device, AT86RF212_REG_VREG_CTRL, &val);
    if ((val & AT86RF212_VREG_CTRL_DVDD_OK_MASK) == 0) {
        AT86RF212_DEBUG_PRINT("DVDD error\r\n");
        return AT86RF212_ERROR_DVDD;
    }

    // Set channel
    res = at86rf212_set_channel(device, AT86RF212_DEFAULT_CHANNEL);
    if (res < 0) {
        AT86RF212_DEBUG_PRINT("Channel set error: %d\r\n", res);
        return AT86RF212_ERROR_DRIVER;
    }

    // Set Clear Channel Assessment (CCA) mode
    res = at86rf212_update_reg(device, AT86RF212_REG_PHY_CC_CCA,
                               AT86RF212_PHY_CC_CCA_CCA_MODE_MASK,
                               AT86RF212_DEFAULT_CCA_MODE << AT86RF212_PHY_CC_CCA_CCA_MODE_SHIFT);
    if (res < 0) {
        AT86RF212_DEBUG_PRINT("CCA mode set error: %d\r\n", res);
        return AT86RF212_ERROR_DRIVER;
    }

    // Enable CSMA-CA
    // Set Binary Exponentials
    res = at86rf212_write_reg(device, AT86RF212_REG_CSMA_BE,
                              (AT86RF212_DEFAULT_MINBE << AT86RF212_CSMA_BE_MIN_SHIFT)
                              | (AT86RF212_DEFAULT_MAXBE << AT86RF212_CSMA_BE_MAX_SHIFT));
    if (res < 0) {
        AT86RF212_DEBUG_PRINT("CSMA exponential set error: %d\r\n", res);
        return AT86RF212_ERROR_DRIVER;
    }

    // Set max CSMA backoffs
    res = at86rf212_update_reg(device, AT86RF212_REG_XAH_CTRL_0,
                               AT86RF212_XAH_CTRL_MAX_CSMA_RETRIES_MASK,
                               AT86RF212_DEFAULT_MAX_CSMA_BACKOFFS << AT86RF212_XAH_CTRL_MAX_CSMA_RETRIES_SHIFT);
    if (res < 0) {
        AT86RF212_DEBUG_PRINT("CSMA max backoff set error: %d\r\n", res);
        return AT86RF212_ERROR_DRIVER;
    }

    // Enable promiscuous mode Auto Ack
    res = at86rf212_update_reg(device, AT86RF212_REG_XAH_CTRL_1,
                               AT86RF212_XAH_CTRL_1_AACK_PROM_MODE_MASK,
                               1 << AT86RF212_XAH_CTRL_1_AACK_PROM_MODE_SHIFT);
    if (res < 0) {
        AT86RF212_DEBUG_PRINT("Promiscuous mode set error: %d\r\n", res);
        return AT86RF212_ERROR_DRIVER;
    }

    // Enable auto CRC for TX
    res = at86rf212_update_reg(device, AT86RF212_REG_TRX_CTRL_1,
                               AT86RF212_TRX_CTRL1_TX_AUTO_CRC_ON_MASK,
                               1 << AT86RF212_TRX_CTRL1_TX_AUTO_CRC_ON_SHIFT);
    if (res < 0) {
        AT86RF212_DEBUG_PRINT("Auto CRC set error: %d\r\n", res);
        return AT86RF212_ERROR_DRIVER;
    }

    // Set IRQ_MASK_MODE to 1
    // This means enabled interrupts will cause IRQ assert, all interrupts
    // can be read from IRQ_STATUS
    res = at86rf212_update_reg(device, AT86RF212_REG_TRX_CTRL_1,
                               AT86RF212_TRX_CTRL1_IRQ_MASK_MODE_MASK,
                               1 << AT86RF212_TRX_CTRL1_IRQ_MASK_MODE_SHIFT);
    if (res < 0) {
        AT86RF212_DEBUG_PRINT("IRQ mask set error: %d\r\n", res);
        return AT86RF212_ERROR_DRIVER;
    }

    // Enable dynamic frame buffer protection
    res = at86rf212_update_reg(device, AT86RF212_REG_TRX_CTRL_2,
                               AT86RF212_TRX_CTRL2_RX_SAFE_MODE_MASK,
                               1 << AT86RF212_TRX_CTRL2_RX_SAFE_MODE_SHIFT);
    if (res < 0) {
        AT86RF212_DEBUG_PRINT("frame buffer protection set error: %d\r\n", res);
        return AT86RF212_ERROR_DRIVER;
    }

    // Enable interrupt pin
    res = at86rf212_set_irq_mask(device, AT86RF212_IRQ_2_RX_START | AT86RF212_IRQ_3_TRX_END);
    if (res < 0) {
        AT86RF212_DEBUG_PRINT("error: %d enabling interrupts\r\n", res);
        return AT86RF212_ERROR_DRIVER;
    }

    //res = at86rf212_set_short_address(device, 0xcafe);

    //res = at86rf212_set_pan_id(device, 0x0100);

    res = at86rf212_set_power_raw(device, 0xC0);

    return AT86RF212_RES_OK;
}

int at86rf212_close(struct at86rf212_s *device)
{
    // TODO: shutdown

    // Clear driver pointer
    device->driver = NULL;
    device->driver_ctx = NULL;

    return AT86RF212_RES_OK;
}

int at86rf212_set_state(struct at86rf212_s *device, uint8_t state)
{
    return at86rf212_update_reg(device, AT86RF212_REG_TRX_STATE, AT86RF212_TRX_STATE_TRX_CMD_MASK, state);
}

int at86rf212_set_state_blocking(struct at86rf212_s *device, uint8_t state)
{
    int res;
    int count = 0;

    // Enable PLL
    res = at86rf212_update_reg(device, AT86RF212_REG_TRX_STATE, AT86RF212_TRX_STATE_TRX_CMD_MASK, state);
    if (res < 0) {
        return res;
    }

    // Block while state change occurs
    while ((state & AT86RF212_TRX_STATUS_TRX_STATUS_MASK) == AT86RF212_STATE_TRANSITION_IN_PROGRESS) {
        res = at86rf212_read_reg(device, AT86RF212_REG_TRX_STATE, &state);
        count ++;
        if (count > AT86RF212_MAX_RETRIES) {
            return AT86RF212_ERROR_RETRIES;
        }
    }

    return AT86RF212_RES_OK;
}

int at86rf212_get_state(struct at86rf212_s *device, uint8_t *state)
{
    int res;
    uint8_t state_int;

    res = at86rf212_read_reg(device, AT86RF212_REG_TRX_STATE, &state_int);
    //TODO: Should we be masking here to only fetch TRX_STATUS or externally so you can also
    // use CCA_STATUS and CCA_DONE? (OR passing additional args for each).
    (*state) = state_int & AT86RF212_TRX_STATUS_TRX_STATUS_MASK;

    return res;
}


int at86rf212_set_channel(struct at86rf212_s *device, uint8_t channel)
{
    return at86rf212_update_reg(device, AT86RF212_REG_PHY_CC_CCA,
                                AT86RF212_PHY_CC_CCA_CHANNEL_MASK,
                                channel << AT86RF212_PHY_CC_CCA_CHANNEL_SHIFT);
}

int at86rf212_get_channel(struct at86rf212_s *device, uint8_t *channel)
{
    int res = at86rf212_read_reg(device, AT86RF212_REG_PHY_CC_CCA, channel);

    *channel = (*channel >> AT86RF212_PHY_CC_CCA_CHANNEL_SHIFT)
               & AT86RF212_PHY_CC_CCA_CHANNEL_MASK;

    return res;
}

int at86rf212_set_irq_mask(struct at86rf212_s *device, uint8_t mask)
{
    return at86rf212_write_reg(device, AT86RF212_REG_IRQ_MASK, mask);
}

int at86rf212_get_irq_status(struct at86rf212_s *device, uint8_t *status)
{
    return at86rf212_read_reg(device, AT86RF212_REG_IRQ_STATUS, status);
}

int at86rf212_set_cca_mode(struct at86rf212_s *device, uint8_t mode)
{
    return at86rf212_update_reg(device, AT86RF212_REG_PHY_CC_CCA,
                                AT86RF212_PHY_CC_CCA_CHANNEL_MASK,
                                mode << AT86RF212_PHY_CC_CCA_CHANNEL_SHIFT);
}

int at86rf212_set_short_address(struct at86rf212_s *device, uint16_t address)
{
    int res;

    res = at86rf212_write_reg(device, AT86RF212_REG_SHORT_ADDR_0, address & 0xFF);
    if (res < 0) {
        return res;
    }
    res = at86rf212_write_reg(device, AT86RF212_REG_SHORT_ADDR_1, (address >> 8) & 0xFF);
    if (res < 0) {
        return res;
    }

    return AT86RF212_RES_OK;
}

int at86rf212_set_pan_id(struct at86rf212_s *device, uint16_t pan_id)
{
    int res;

    res = at86rf212_write_reg(device, AT86RF212_REG_PAN_ID_0, pan_id & 0xFF);
    if (res < 0) {
        return res;
    }
    res = at86rf212_write_reg(device, AT86RF212_REG_PAN_ID_1, (pan_id >> 8) & 0xFF);
    if (res < 0) {
        return res;
    }

    return AT86RF212_RES_OK;
}

int at86rf212_set_power_raw(struct at86rf212_s *device, uint8_t power)
{
    return at86rf212_update_reg(device, AT86RF212_REG_PHY_TX_PWR,
                                AT86RF212_PHY_TX_PWR_TX_PWR_MASK,
                                power << AT86RF212_PHY_TX_PWR_TX_PWR_SHIFT);
}

int at86rf212_start_rx(struct at86rf212_s *device)
{
    int res;
    uint8_t irq;

    // TODO: ensure PLL is enabled

    // Reset state
    res = at86rf212_set_state_blocking(device, AT86RF212_CMD_TRX_OFF);
    if (res < 0) {
        AT86RF212_DEBUG_PRINT("Timeout setting TRX OFF state\r\n");
        return res;
    }

    PLATFORM_SLEEP_US(1);

    // Clear interrupts
    res = at86rf212_get_irq_status(device, &irq);
    if (res < 0) {
        return res;
    }

    // Enable PLL
    res = at86rf212_set_state_blocking(device, AT86RF212_CMD_PLL_ON);
    if (res < 0) {
        AT86RF212_DEBUG_PRINT("Timeout setting PLL ON state\r\n");
        return res;
    }

    // Await PLL lock
    for (int i = 0; i < AT86RF212_PLL_LOCK_RETRIES; i++) {
        res = at86rf212_get_irq_status(device, &irq);
        if (res < 0) {
            return res;
        }
        if ((irq & AT86RF212_IRQ_STATUS_IRQ_0_PLL_LOCK_MASK) != 0) {
            break;
        }
        PLATFORM_SLEEP_US(1);
    }

    if ((irq & AT86RF212_IRQ_STATUS_IRQ_0_PLL_LOCK_MASK) == 0) {
        AT86RF212_DEBUG_PRINT("Timeout awaiting PLL lock (IRQ status: 0x%x)\r\n", irq);
        return AT86RF212_ERROR_PLL;
    }

    // Enable RX mode
    res = at86rf212_set_state_blocking(device, AT86RF212_CMD_RX_ON);
    if (res < 0) {
        return res;
    }

    return AT86RF212_RES_OK;
}

int at86rf212_check_rx(struct at86rf212_s *device)
{
    int res;
    uint8_t irq;

    res = at86rf212_read_reg(device, AT86RF212_REG_IRQ_STATUS, &irq);
    if (res < 0) {
        return AT86RF212_ERROR_DRIVER;
    }

    if ((irq & AT86RF212_IRQ_STATUS_IRQ_3_TRX_END_MASK) != 0) {
        return AT86RF212_RES_DONE;
    }

    return AT86RF212_RES_OK;
}

int at86rf212_get_rx(struct at86rf212_s *device, uint8_t* length, uint8_t* data)
{
    int res;
    uint8_t buffer[AT86RF212_LEN_FIELD_LEN + AT86RF212_MAX_LENGTH + AT86RF212_FRAME_RX_OVERHEAD];

    // Check CRC
    // AT86RF212_REG_PHY_RSSI & 0x80 != 0

    // Fetch frame length
    uint8_t frame_len;
    res = at86rf212_read_frame(device, AT86RF212_LEN_FIELD_LEN, &frame_len);
    if (res < 0) {
        return AT86RF212_ERROR_DRIVER;
    }

    AT86RF212_DEBUG_PRINT("Frame length: %d\r\n", frame_len);

    // Check frame length is valid
    if (frame_len > AT86RF212_MAX_LENGTH) {
        return AT86RF212_ERROR_LEN;
    }

    // Read frame from buffer
    res = at86rf212_read_frame(device, AT86RF212_LEN_FIELD_LEN + frame_len + AT86RF212_FRAME_RX_OVERHEAD, buffer);

    // Copy buffer out
    // TODO: should we parse the additional fields here or outside of this function?
    *length = frame_len + AT86RF212_FRAME_RX_OVERHEAD;
    memcpy(data, buffer + 1, frame_len + AT86RF212_FRAME_RX_OVERHEAD);

    // TODO: clear buffer?


    return res;
}

int at86rf212_start_tx(struct at86rf212_s *device, uint8_t length, uint8_t* data)
{
    int res;
    uint8_t irq;

    uint8_t send_data[AT86RF212_MAX_LENGTH];

    // Reset state
    res = at86rf212_set_state_blocking(device, AT86RF212_CMD_TRX_OFF);
    if (res < 0) {
        AT86RF212_DEBUG_PRINT("Timeout setting TRX OFF state\r\n");
        return res;
    }

    PLATFORM_SLEEP_US(1);

    // Clear interrupts
    res = at86rf212_get_irq_status(device, &irq);
    if (res < 0) {
        return res;
    }

    // Enable PLL
    res = at86rf212_set_state_blocking(device, AT86RF212_CMD_PLL_ON);
    if (res < 0) {
        AT86RF212_DEBUG_PRINT("Timeout setting PLL ON state\r\n");
        return res;
    }

    // Await PLL lock
    for (int i = 0; i < AT86RF212_PLL_LOCK_RETRIES; i++) {
        res = at86rf212_get_irq_status(device, &irq);
        if (res < 0) {
            return res;
        }
        if ((irq & AT86RF212_IRQ_STATUS_IRQ_0_PLL_LOCK_MASK) != 0) {
            break;
        }
        PLATFORM_SLEEP_US(1);
    }

    if ((irq & AT86RF212_IRQ_STATUS_IRQ_0_PLL_LOCK_MASK) == 0) {
        AT86RF212_DEBUG_PRINT("Timeout awaiting PLL lock (IRQ status: 0x%x)\r\n", irq);
        return AT86RF212_ERROR_PLL;
    }

    // Create data frame for writing
    send_data[0] = length;
    for (int i = 0; i < length; i++) {
        send_data[i + 1] = data[i];
    }
    send_data[length + 1] = 0x00;
    send_data[length + 2] = 0x00;

    // Write frame to device
    // Note that data[0] must be length - AT86RF212_LEN_FIELD_LEN
    res = at86rf212_write_frame(device, length + AT86RF212_LEN_FIELD_LEN + AT86RF212_CRC_LEN, send_data);
    if (res < 0) {
        return res;
    }

#if 0
    //TODO: not currently using auto ack mode
    res = at86rf212_set_state_blocking(device, AT86RF212_CMD_TX_ARET_ON);
    if (res < 0) {
        AT86RF212_DEBUG_PRINT("Timeout setting TX_ARET_ON\r\n", irq);
        return res;
    }
#endif

#if 1
    // Send command to start transmission
    res = at86rf212_set_state_blocking(device, AT86RF212_CMD_TX_START);
    if (res < 0) {
        AT86RF212_DEBUG_PRINT("Timeout setting TRX_START\r\n");
        return res;
    }
#else
    // Assert SLP_TR pin to trigger transmission
    device->driver->set_slp_tr(device->driver_ctx, 1);
    PLATFORM_SLEEP_MS(1);
    device->driver->set_slp_tr(device->driver_ctx, 0);
#endif

    return AT86RF212_RES_OK;
}

int at86rf212_check_tx(struct at86rf212_s *device)
{
    int res;
    uint8_t irq;

#if 0
    // TODO: if IRQ pin is enabled can poll or interrupt on this.
    // This code should however be in the application, not the driver
    res = device->driver->get_irq(device->driver_ctx, &irq);
    if (res < 0) {
        return AT86RF212_ERROR_DRIVER;
    }
    if (irq == 0) {
        return AT86RF212_RES_OK;
    }
#endif

    res = at86rf212_read_reg(device, AT86RF212_REG_IRQ_STATUS, &irq);
    if (res < 0) {
        return AT86RF212_ERROR_DRIVER;
    }

    if ((irq & AT86RF212_IRQ_STATUS_IRQ_3_TRX_END_MASK) != 0) {
        return AT86RF212_RES_DONE;
    }

    return AT86RF212_RES_OK;
}

