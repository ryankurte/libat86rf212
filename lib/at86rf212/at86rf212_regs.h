/*
 * mpu9250 registers for internal library use
 *
 * Copyright 2016 Ryan Kurte
 */

#ifndef AT86RF212_REGS_H
#define AT86RF212_REGS_H

#ifdef __cplusplus
extern "C" {
#endif


/** Base address for transceiver **/
#define AT86RF212_REG_BASE_ADDR    (0x00)   //!< Register base address
#define AT86RF212_AES_BASE_ADDR    (0x80)  //!< AES base address


// AT86RF212 Register Enumeration
enum at86rf212_reg_e {
    AT86RF212_REG_AES_CTRL        = 0x03,   //!< Offset for register AES_CTRL
    AT86RF212_REG_AES_CTRL_MIRROR = 0x14,   //!< Offset for register AES_CTRL_MIRROR
    AT86RF212_REG_AES_STATE_KEY_0 = 0x04,   //!< Offset for register AES_STATE_KEY_0
    AT86RF212_REG_AES_STATUS      = 0x02,   //!< Offset for register AES_STATUS
    AT86RF212_REG_ANT_DIV         = 0x0D,   //!< Offset for register ANT_DIV
    AT86RF212_REG_BATMON          = 0x11,   //!< Offset for register BATMON
    AT86RF212_REG_CCA_THRES       = 0x09,   //!< Offset for register CCA_THRES
    AT86RF212_REG_CC_CTRL_0       = 0x13,   //!< Offset for register CC_CTRL_0
    AT86RF212_REG_CC_CTRL_1       = 0x14,   //!< Offset for register CC_CTRL_1
    AT86RF212_REG_CSMA_BE         = 0x2F,   //!< Offset for register CSMA_BE
    AT86RF212_REG_CSMA_SEED_0     = 0x2D,   //!< Offset for register CSMA_SEED_0
    AT86RF212_REG_CSMA_SEED_1     = 0x2E,   //!< Offset for register CSMA_SEED_1
    AT86RF212_REG_FTN_CTRL        = 0x18,   //!< Offset for register FTN_CTRL
    AT86RF212_REG_IEEE_ADDR_0     = 0x24,   //!< Offset for register IEEE_ADDR_0
    AT86RF212_REG_IEEE_ADDR_1     = 0x25,   //!< Offset for register IEEE_ADDR_1
    AT86RF212_REG_IEEE_ADDR_2     = 0x26,   //!< Offset for register IEEE_ADDR_2
    AT86RF212_REG_IEEE_ADDR_3     = 0x27,   //!< Offset for register IEEE_ADDR_3
    AT86RF212_REG_IEEE_ADDR_4     = 0x28,   //!< Offset for register IEEE_ADDR_4
    AT86RF212_REG_IEEE_ADDR_5     = 0x29,   //!< Offset for register IEEE_ADDR_5
    AT86RF212_REG_IEEE_ADDR_6     = 0x2A,   //!< Offset for register IEEE_ADDR_6
    AT86RF212_REG_IEEE_ADDR_7     = 0x2B,   //!< Offset for register IEEE_ADDR_7
    AT86RF212_REG_IRQ_MASK        = 0x0E,   //!< Offset for register IRQ_MASK
    AT86RF212_REG_IRQ_STATUS      = 0x0F,   //!< Offset for register IRQ_STATUS
    AT86RF212_REG_MAN_ID_0        = 0x1E,   //!< Offset for register MAN_ID_0
    AT86RF212_REG_MAN_ID_1        = 0x1F,   //!< Offset for register MAN_ID_1
    AT86RF212_REG_PAN_ID_0        = 0x22,   //!< Offset for register PAN_ID_0
    AT86RF212_REG_PAN_ID_1        = 0x23,   //!< Offset for register PAN_ID_1
    AT86RF212_REG_PART_NUM        = 0x1C,   //!< Offset for register PART_NUM
    AT86RF212_REG_PHY_CC_CCA      = 0x08,   //!< Offset for register PHY_CC_CCA
    AT86RF212_REG_PHY_ED_LEVEL    = 0x07,   //!< Offset for register PHY_ED_LEVEL
    AT86RF212_REG_PHY_RSSI        = 0x06,   //!< Offset for register PHY_RSSI
    AT86RF212_REG_PHY_TX_PWR      = 0x05,   //!< Offset for register PHY_TX_PWR
    AT86RF212_REG_PLL_CF          = 0x1A,   //!< Offset for register PLL_CF
    AT86RF212_REG_PLL_DCU         = 0x1B,   //!< Offset for register PLL_DCU
    AT86RF212_REG_RF_CTRL_0       = 0x16,   //!< Offset for register RF_CTRL_0
    AT86RF212_REG_RF_CTRL_1       = 0x19,   //!< Offset for register RF_CTRL_1
    AT86RF212_REG_RX_CTRL         = 0x0A,   //!< Offset for register RX_CTRL
    AT86RF212_REG_RX_SYN          = 0x15,   //!< Offset for register RX_SYN
    AT86RF212_REG_SFD_VALUE       = 0x0B,   //!< Offset for register SFD_VALUE
    AT86RF212_REG_SHORT_ADDR_0    = 0x20,   //!< Offset for register SHORT_ADDR_0
    AT86RF212_REG_SHORT_ADDR_1    = 0x21,   //!< Offset for register SHORT_ADDR_1
    AT86RF212_REG_TRX_CTRL_0      = 0x03,   //!< Offset for register TRX_CTRL_0
    AT86RF212_REG_TRX_CTRL_1      = 0x04,   //!< Offset for register TRX_CTRL_1
    AT86RF212_REG_TRX_CTRL_2      = 0x0C,   //!< Offset for register TRX_CTRL_2
    AT86RF212_REG_TRX_STATE       = 0x02,   //!< Offset for register TRX_STATE
    AT86RF212_REG_TRX_STATUS      = 0x01,   //!< Offset for register TRX_STATUS
    AT86RF212_REG_VERSION_NUM     = 0x1D,   //!< Offset for register VERSION_NUM
    AT86RF212_REG_VREG_CTRL       = 0x10,   //!< Offset for register VREG_CTRL
    AT86RF212_REG_XAH_CTRL_0      = 0x2C,   //!< Offset for register XAH_CTRL_0
    AT86RF212_REG_XAH_CTRL_1      = 0x17,   //!< Offset for register XAH_CTRL_1
    AT86RF212_REG_XOSC_CTRL       = 0x12,   //!< Offset for register XOSC_CTRL
};

// REG_BLANK
#define AT86RF212_1_MASK       0x01
#define AT86RF212_1_SHIFT      0
#define AT86RF212_2_MASK       0x02
#define AT86RF212_2_SHIFT      1
#define AT86RF212_3_MASK       0x04
#define AT86RF212_3_SHIFT      2
#define AT86RF212_4_MASK       0x08
#define AT86RF212_4_SHIFT      3
#define AT86RF212_5_MASK       0x10
#define AT86RF212_5_SHIFT      4
#define AT86RF212_6_MASK       0x20
#define AT86RF212_6_SHIFT      5
#define AT86RF212_7_MASK       0x40
#define AT86RF212_7_SHIFT      6
#define AT86RF212_8_MASK       0x80
#define AT86RF212_8_SHIFT      7

// TRX_STATUS register
#define AT86RF212_TRX_STATUS_TRX_STATUS_MASK    0x1F
#define AT86RF212_TRX_STATUS_CCA_STATUS_MASK    0x40
#define AT86RF212_TRX_STATUS_CCA_DONE_MASK      0x80

// TRX_STATE register
#define AT86RF212_TRX_STATE_TRX_CMD_MASK       0x1F
#define AT86RF212_TRX_STATE_TRX_CMD_SHIFT      0
#define AT86RF212_TRX_STATE_TRAC_STATUS_MASK   0xA0
#define AT86RF212_TRX_STATE_TRAC_STATUS_SHIFT  5

// TRX_CTRL0
#define AT86RF212_TRX_CTRL0_CLKM_CTRL_MASK     0x07
#define AT86RF212_TRX_CTRL0_CLKM_CTRL_SHIFT    0
#define AT86RF212_TRX_CTRL0_CLKM_SHA_SEL_MASK  0x08
#define AT86RF212_TRX_CTRL0_CLKM_SHA_SEL_SHIFT 3
#define AT86RF212_TRX_CTRL0_PAD_IO_CLKM_MASK   0x30
#define AT86RF212_TRX_CTRL0_PAD_IO_CLKM_SHIFT  4
#define AT86RF212_TRX_CTRL0_PAD_IO_MASK        0xC0
#define AT86RF212_TRX_CTRL0_PAD_IO_SHIFT       6

// TRX_CTRL1
#define AT86RF212_TRX_CTRL1_IRQ_POLARITY_MASK       0x01
#define AT86RF212_TRX_CTRL1_IRQ_POLARITY_SHIFT      0
#define AT86RF212_TRX_CTRL1_IRQ_MASK_MODE_MASK      0x02
#define AT86RF212_TRX_CTRL1_IRQ_MASK_MODE_SHIFT     1
#define AT86RF212_TRX_CTRL1_SPI_CMD_MODE_MASK       0x0C
#define AT86RF212_TRX_CTRL1_SPI_CMD_MODE_SHIFT      2
#define AT86RF212_TRX_CTRL1_RX_BL_CTRL_MASK         0x10
#define AT86RF212_TRX_CTRL1_RX_BL_CTRL_SHIFT        4
#define AT86RF212_TRX_CTRL1_TX_AUTO_CRC_ON_MASK     0x20
#define AT86RF212_TRX_CTRL1_TX_AUTO_CRC_ON_SHIFT    5
#define AT86RF212_TRX_CTRL1_IRQ_2_EXT_EN_MASK       0x40
#define AT86RF212_TRX_CTRL1_IRQ_2_EXT_EN_SHIFT      6
#define AT86RF212_TRX_CTRL1_PA_EXT_EN_MASK          0x80
#define AT86RF212_TRX_CTRL1_PA_EXT_EN_SHIFT         7

// PHY_CC_CCA
#define AT86RF212_PHY_CC_CCA_CHANNEL_MASK       0x1F
#define AT86RF212_PHY_CC_CCA_CHANNEL_SHIFT      0
#define AT86RF212_PHY_CC_CCA_CCA_MODE_MASK      0x40
#define AT86RF212_PHY_CC_CCA_CCA_MODE_SHIFT     5
#define AT86RF212_PHY_CC_CCA_CCA_REQ_MASK       0x80
#define AT86RF212_PHY_CC_CCA_CCA_REQ_SHIFT      7

// IRQ_STATUS
#define AT86RF212_IRQ_STATUS_IRQ_7_BAT_LOW_MASK         0x01
#define AT86RF212_IRQ_STATUS_IRQ_7_BAT_LOW_SHIFT        0
#define AT86RF212_IRQ_STATUS_IRQ_6_TRX_UR_MASK          0x02
#define AT86RF212_IRQ_STATUS_IRQ_6_TRX_UR_SHIFT         1
#define AT86RF212_IRQ_STATUS_IRQ_5_AMI_MASK             0x04
#define AT86RF212_IRQ_STATUS_IRQ_5_AMI_SHIFT            2
#define AT86RF212_IRQ_STATUS_IRQ_4_CCA_ED_DONE_MASK     0x08
#define AT86RF212_IRQ_STATUS_IRQ_4_CCA_ED_DONE_SHIFT    3
#define AT86RF212_IRQ_STATUS_IRQ_3_TRX_END_MASK         0x10
#define AT86RF212_IRQ_STATUS_IRQ_3_TRX_END_SHIFT        4
#define AT86RF212_IRQ_STATUS_IRQ_2_RX_START_MASK        0x20
#define AT86RF212_IRQ_STATUS_IRQ_2_RX_START_SHIFT       5
#define AT86RF212_IRQ_STATUS_IRQ_1_PLL_UNLOCK_MASK      0x40
#define AT86RF212_IRQ_STATUS_IRQ_1_PLL_UNLOCK_SHIFT     6
#define AT86RF212_IRQ_STATUS_IRQ_0_PLL_LOCK_MASK        0x80
#define AT86RF212_IRQ_STATUS_IRQ_0_PLL_LOCK_SHIFT       7


#ifdef __cplusplus
}
#endif

#endif

