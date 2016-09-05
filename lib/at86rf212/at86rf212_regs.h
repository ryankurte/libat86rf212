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
#define AT86RF212_AES_BASE_ADDR    (0x80)   //!< AES base address


// AT86RF212 Register Enumeration
enum at86rf212_reg_e {
    AT86RF212_REG_TRX_STATUS      = 0x01,   //!< TRX_STATUS register address
    AT86RF212_REG_AES_STATUS      = 0x02,   //!< AES_STATUS register address
    AT86RF212_REG_TRX_STATE       = 0x02,   //!< TRX_STATE register address
    AT86RF212_REG_TRX_CTRL_0      = 0x03,   //!< TRX_CTRL_0 register address
    AT86RF212_REG_TRX_CTRL_1      = 0x04,   //!< TRX_CTRL_1 register address
    AT86RF212_REG_PHY_TX_PWR      = 0x05,   //!< PHY_TX_PWR register address
    AT86RF212_REG_PHY_RSSI        = 0x06,   //!< PHY_RSSI register address
    AT86RF212_REG_PHY_ED_LEVEL    = 0x07,   //!< PHY_ED_LEVEL register address
    AT86RF212_REG_PHY_CC_CCA      = 0x08,   //!< PHY_CC_CCA register address
    AT86RF212_REG_CCA_THRES       = 0x09,   //!< CCA_THRES register address
    AT86RF212_REG_RX_CTRL         = 0x0A,   //!< RX_CTRL register address
    AT86RF212_REG_SFD_VALUE       = 0x0B,   //!< SFD_VALUE register address
    AT86RF212_REG_TRX_CTRL_2      = 0x0C,   //!< TRX_CTRL_2 register address
    AT86RF212_REG_ANT_DIV         = 0x0D,   //!< ANT_DIV register address
    AT86RF212_REG_IRQ_MASK        = 0x0E,   //!< IRQ_MASK register address
    AT86RF212_REG_IRQ_STATUS      = 0x0F,   //!< IRQ_STATUS register address
    AT86RF212_REG_VREG_CTRL       = 0x10,   //!< VREG_CTRL register address
    AT86RF212_REG_BATMON          = 0x11,   //!< BATMON register address
    AT86RF212_REG_XOSC_CTRL       = 0x12,   //!< XOSC_CTRL register address
    AT86RF212_REG_CC_CTRL_0       = 0x13,   //!< CC_CTRL_0 register address
    AT86RF212_REG_CC_CTRL_1       = 0x14,   //!< CC_CTRL_1 register address
    AT86RF212_REG_RX_SYN          = 0x15,   //!< RX_SYN register address
    AT86RF212_REG_RF_CTRL_0       = 0x16,   //!< RF_CTRL_0 register address
    AT86RF212_REG_XAH_CTRL_1      = 0x17,   //!< XAH_CTRL_1 register address
    AT86RF212_REG_FTN_CTRL        = 0x18,   //!< FTN_CTRL register address
    AT86RF212_REG_RF_CTRL_1       = 0x19,   //!< RF_CTRL_1 register address
    AT86RF212_REG_PLL_CF          = 0x1A,   //!< PLL_CF register address
    AT86RF212_REG_PLL_DCU         = 0x1B,   //!< PLL_DCU register address
    AT86RF212_REG_PART_NUM        = 0x1C,   //!< PART_NUM register address
    AT86RF212_REG_VERSION_NUM     = 0x1D,   //!< VERSION_NUM register address
    AT86RF212_REG_MAN_ID_0        = 0x1E,   //!< MAN_ID_0 register address
    AT86RF212_REG_MAN_ID_1        = 0x1F,   //!< MAN_ID_1 register address
    AT86RF212_REG_SHORT_ADDR_0    = 0x20,   //!< SHORT_ADDR_0 register address
    AT86RF212_REG_SHORT_ADDR_1    = 0x21,   //!< SHORT_ADDR_1 register address
    AT86RF212_REG_PAN_ID_0        = 0x22,   //!< PAN_ID_0 register address
    AT86RF212_REG_PAN_ID_1        = 0x23,   //!< PAN_ID_1 register address
    AT86RF212_REG_IEEE_ADDR_0     = 0x24,   //!< IEEE_ADDR_0 register address
    AT86RF212_REG_IEEE_ADDR_1     = 0x25,   //!< IEEE_ADDR_1 register address
    AT86RF212_REG_IEEE_ADDR_2     = 0x26,   //!< IEEE_ADDR_2 register address
    AT86RF212_REG_IEEE_ADDR_3     = 0x27,   //!< IEEE_ADDR_3 register address
    AT86RF212_REG_IEEE_ADDR_4     = 0x28,   //!< IEEE_ADDR_4 register address
    AT86RF212_REG_IEEE_ADDR_5     = 0x29,   //!< IEEE_ADDR_5 register address
    AT86RF212_REG_IEEE_ADDR_6     = 0x2A,   //!< IEEE_ADDR_6 register address
    AT86RF212_REG_IEEE_ADDR_7     = 0x2B,   //!< IEEE_ADDR_7 register address
    AT86RF212_REG_XAH_CTRL_0      = 0x2C,   //!< XAH_CTRL_0 register address
    AT86RF212_REG_CSMA_SEED_0     = 0x2D,   //!< CSMA_SEED_0 register address
    AT86RF212_REG_CSMA_SEED_1     = 0x2E,   //!< CSMA_SEED_1 register address
    AT86RF212_REG_CSMA_BE         = 0x2F,   //!< CSMA_BE register address
};

// Register template to speed up addition of new registers
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
#define AT86RF212_TRX_STATUS_TRX_STATUS_MASK            0x1F
#define AT86RF212_TRX_STATUS_CCA_STATUS_MASK            0x40
#define AT86RF212_TRX_STATUS_CCA_DONE_MASK              0x80

// TRX_STATE register
#define AT86RF212_TRX_STATE_TRX_CMD_MASK                0x1F
#define AT86RF212_TRX_STATE_TRX_CMD_SHIFT               0
#define AT86RF212_TRX_STATE_TRAC_STATUS_MASK            0xA0
#define AT86RF212_TRX_STATE_TRAC_STATUS_SHIFT           5

// TRX_CTRL0
#define AT86RF212_TRX_CTRL0_CLKM_CTRL_MASK              0x07
#define AT86RF212_TRX_CTRL0_CLKM_CTRL_SHIFT             0
#define AT86RF212_TRX_CTRL0_CLKM_SHA_SEL_MASK           0x08
#define AT86RF212_TRX_CTRL0_CLKM_SHA_SEL_SHIFT          3
#define AT86RF212_TRX_CTRL0_PAD_IO_CLKM_MASK            0x30
#define AT86RF212_TRX_CTRL0_PAD_IO_CLKM_SHIFT           4
#define AT86RF212_TRX_CTRL0_PAD_IO_MASK                 0xC0
#define AT86RF212_TRX_CTRL0_PAD_IO_SHIFT                6

// TRX_CTRL1
#define AT86RF212_TRX_CTRL1_IRQ_POLARITY_MASK           0x01
#define AT86RF212_TRX_CTRL1_IRQ_POLARITY_SHIFT          0
#define AT86RF212_TRX_CTRL1_IRQ_MASK_MODE_MASK          0x02
#define AT86RF212_TRX_CTRL1_IRQ_MASK_MODE_SHIFT         1
#define AT86RF212_TRX_CTRL1_SPI_CMD_MODE_MASK           0x0C
#define AT86RF212_TRX_CTRL1_SPI_CMD_MODE_SHIFT          2
#define AT86RF212_TRX_CTRL1_RX_BL_CTRL_MASK             0x10
#define AT86RF212_TRX_CTRL1_RX_BL_CTRL_SHIFT            4
#define AT86RF212_TRX_CTRL1_TX_AUTO_CRC_ON_MASK         0x20
#define AT86RF212_TRX_CTRL1_TX_AUTO_CRC_ON_SHIFT        5
#define AT86RF212_TRX_CTRL1_IRQ_2_EXT_EN_MASK           0x40
#define AT86RF212_TRX_CTRL1_IRQ_2_EXT_EN_SHIFT          6
#define AT86RF212_TRX_CTRL1_PA_EXT_EN_MASK              0x80
#define AT86RF212_TRX_CTRL1_PA_EXT_EN_SHIFT             7

// TRX_CTRL2
#define AT86RF212_TRX_CTRL2_OQPSK_DATA_RATE_MASK        0x02
#define AT86RF212_TRX_CTRL2_OQPSK_DATA_RATE_SHIFT       0
#define AT86RF212_TRX_CTRL2_SUB_MODE_MASK               0x04
#define AT86RF212_TRX_CTRL2_SUB_MODE_SHIFT              2
#define AT86RF212_TRX_CTRL2_BPSK_OQPSK_MASK             0x08
#define AT86RF212_TRX_CTRL2_BPSK_OQPSK_SHIFT            3
#define AT86RF212_TRX_CTRL2_ALT_SPECTRUM_MASK           0x10
#define AT86RF212_TRX_CTRL2_ALT_SPECTRUM_SHIFT          4
#define AT86RF212_TRX_CTRL2_OQPSK_SCRAM_EN_MASK         0x20
#define AT86RF212_TRX_CTRL2_OQPSK_SCRAM_EN_SHIFT        5
#define AT86RF212_TRX_CTRL2_TRX_OFF_AVDD_EN_MASK        0x40
#define AT86RF212_TRX_CTRL2_TRX_OFF_AVDD_EN_SHIFT       6
#define AT86RF212_TRX_CTRL2_RX_SAFE_MODE_MASK           0x80
#define AT86RF212_TRX_CTRL2_RX_SAFE_MODE_SHIFT          7


// PHY_CC_CCA
#define AT86RF212_PHY_CC_CCA_CHANNEL_MASK               0x1F
#define AT86RF212_PHY_CC_CCA_CHANNEL_SHIFT              0
#define AT86RF212_PHY_CC_CCA_CCA_MODE_MASK              0x40
#define AT86RF212_PHY_CC_CCA_CCA_MODE_SHIFT             5
#define AT86RF212_PHY_CC_CCA_CCA_REQ_MASK               0x80
#define AT86RF212_PHY_CC_CCA_CCA_REQ_SHIFT              7

// IRQ_STATUS
#define AT86RF212_IRQ_STATUS_IRQ_0_PLL_LOCK_MASK        0x01
#define AT86RF212_IRQ_STATUS_IRQ_0_PLL_LOCK_SHIFT       0
#define AT86RF212_IRQ_STATUS_IRQ_1_PLL_UNLOCK_MASK      0x02
#define AT86RF212_IRQ_STATUS_IRQ_1_PLL_UNLOCK_SHIFT     1
#define AT86RF212_IRQ_STATUS_IRQ_2_RX_START_MASK        0x04
#define AT86RF212_IRQ_STATUS_IRQ_2_RX_START_SHIFT       2
#define AT86RF212_IRQ_STATUS_IRQ_3_TRX_END_MASK         0x08
#define AT86RF212_IRQ_STATUS_IRQ_3_TRX_END_SHIFT        3
#define AT86RF212_IRQ_STATUS_IRQ_4_CCA_ED_DONE_MASK     0x10
#define AT86RF212_IRQ_STATUS_IRQ_4_CCA_ED_DONE_SHIFT    4
#define AT86RF212_IRQ_STATUS_IRQ_5_AMI_MASK             0x20
#define AT86RF212_IRQ_STATUS_IRQ_5_AMI_SHIFT            5
#define AT86RF212_IRQ_STATUS_IRQ_6_TRX_UR_MASK          0x40
#define AT86RF212_IRQ_STATUS_IRQ_6_TRX_UR_SHIFT         6
#define AT86RF212_IRQ_STATUS_IRQ_7_BAT_LOW_MASK         0x80
#define AT86RF212_IRQ_STATUS_IRQ_7_BAT_LOW_SHIFT        7


// CSMA_BE
#define AT86RF212_CSMA_BE_MIN_MASK                      0x0F
#define AT86RF212_CSMA_BE_MIN_SHIFT                     0
#define AT86RF212_CSMA_BE_MAX_MASK                      0xF0
#define AT86RF212_CSMA_BE_MAX_SHIFT                     4

// XAH_CTRL_0
#define AT86RF212_XAH_CTRL_SLOTTED_OPERATION_MASK       0x01
#define AT86RF212_XAH_CTRL_SLOTTED_OPERATION_SHIFT      0
#define AT86RF212_XAH_CTRL_MAX_CSMA_RETRIES_MASK        0x0E
#define AT86RF212_XAH_CTRL_MAX_CSMA_RETRIES_SHIFT       1
#define AT86RF212_XAH_CTRL_MAX_FRAME_RETRIES_MASK       0xF0
#define AT86RF212_XAH_CTRL_MAX_FRAME_RETRIES_SHIFT      4

// XAH_CTRL_1
#define AT86RF212_XAH_CTRL_1_AACK_PROM_MODE_MASK        0x02
#define AT86RF212_XAH_CTRL_1_AACK_PROM_MODE_SHIFT       1
#define AT86RF212_XAH_CTRL_1_AACK_ACK_TIME_MASK         0x04
#define AT86RF212_XAH_CTRL_1_AACK_ACK_TIME_SHIFT        2
#define AT86RF212_XAH_CTRL_1_AACK_UPLD_RES_FT_MASK      0x10
#define AT86RF212_XAH_CTRL_1_AACK_UPLD_RES_FT_SHIFT     4
#define AT86RF212_XAH_CTRL_1_AACK_FLTR_RES_FT_MASK      0x20
#define AT86RF212_XAH_CTRL_1_AACK_FLTR_RES_FT_SHIFT     5
#define AT86RF212_XAH_CTRL_1_CSMA_LBT_MODE_MASK         0x40
#define AT86RF212_XAH_CTRL_1_CSMA_LBT_MODE_SHIFT        6

// VREG_CTRL
#define AT86RF212_VREG_CTRL_DVDD_OK_MASK                0x04
#define AT86RF212_VREG_CTRL_DVDD_OK_SHIFT               2
#define AT86RF212_VREG_CTRL_DVREG_EXT_MASK              0x08
#define AT86RF212_VREG_CTRL_DVREG_EXT_SHIFT             3
#define AT86RF212_VREG_CTRL_AVDD_OK_MASK                0x40
#define AT86RF212_VREG_CTRL_AVDD_OK_SHIFT               6
#define AT86RF212_VREG_CTRL_AVREG_EXT_MASK              0x80
#define AT86RF212_VREG_CTRL_AVREG_EXT_SHIFT             7

// BATMON
#define AT86RF212_BATMON_BATMON_VTH_MASK                0x0F
#define AT86RF212_BATMON_BATMON_VTH_SHIFT               0
#define AT86RF212_BATMON_BATMON_HR_MASK                 0x10
#define AT86RF212_BATMON_BATMON_HR_SHIFT                4
#define AT86RF212_BATMON_BATMON_OK_MASK                 0x20
#define AT86RF212_BATMON_BATMON_OK_SHIFT                5
#define AT86RF212_BATMON_PLL_LOCK_MASK                  0x80
#define AT86RF212_BATMON_PLL_LOCK_SHIFT                 7

// PHY_TX_PWR
#define AT86RF212_PHY_TX_PWR_TX_PWR_MASK                0x1F
#define AT86RF212_PHY_TX_PWR_TX_PWR_SHIFT               0
#define AT86RF212_PHY_TX_PWR_GC_PA_MASK                 0x60
#define AT86RF212_PHY_TX_PWR_GC_PA_SHIFT                5
#define AT86RF212_PHY_TX_PWR_PA_BOOST_MASK              0x80
#define AT86RF212_PHY_TX_PWR_PA_BOOST_SHIFT             7

#ifdef __cplusplus
}
#endif

#endif

