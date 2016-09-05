/*
 * at86rf212 related definitions for internal library use
 *
 * Copyright 2016 Ryan Kurte
 */

#ifndef AT86RF212_DEFS_H
#define AT86RF212_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

// SPI control flags
#define AT86RF212_REG_WRITE_FLAG     (0xC0)
#define AT86RF212_REG_READ_FLAG      (0x80)
#define AT86RF212_FRAME_WRITE_FLAG   (0x60)
#define AT86RF212_FRAME_READ_FLAG    (0x20)
#define AT86RF212_SRAM_WRITE_FLAG    (0x40)
#define AT86RF212_SRAM_READ_FLAG     (0x00)

// Device constants
#define AT86RF212_MAX_LENGTH         (127)  //!< Maximum packet length
#define AT86RF212_LEN_FIELD_LEN      1      //!< Length of the PDSU length field
#define AT86RF212_CRC_LEN            2      //!< Length of the CRC field
#define AT86RF212_FRAME_RX_OVERHEAD  3      //!< Number of additional bytes read from frame buffer on RX

/** Enumerations */

/** sub-register TRX_CMD in register TRX_STATE */
enum at86rf212_trx_cmd_e {
    AT86RF212_CMD_NOP           = 0x00,   //!< No-op for reading SPI status (configurable)
    AT86RF212_CMD_TX_START      = 0x02,   //!< Start TX
    AT86RF212_CMD_FORCE_TRX_OFF = 0x03,   //!< Force disable TRX module
    AT86RF212_CMD_FORCE_PLL_ON  = 0x04,   //!< Force enable PLL
    AT86RF212_CMD_RX_ON         = 0x06,   //!< Enable RX
    AT86RF212_CMD_TRX_OFF       = 0x08,   //!< Disable TRX module
    AT86RF212_CMD_PLL_ON        = 0x09,   //!< Enable PLL
    AT86RF212_CMD_RX_AACK_ON    = 0x16,   //!< Enable RX with Auto-Ack
    AT86RF212_CMD_TX_ARET_ON    = 0x19,   //!< Enable TX with Auto-Retry
    AT86RF212_CMD_SLEEP         = 0x0F,   //!< Switch to sleep mode
};


/** sub-register TRX_STATUS in register TRX_STATUS */
enum at86rf212_tal_trx_status_e {
    AT86RF212_P_ON                          = 0x00,   //!< Constant P_ON for sub-register @ref SR_TRX_STATUS
    AT86RF212_BUSY_RX                       = 0x01,   //!< Busy receiving
    AT86RF212_BUSY_TX                       = 0x02,   //!< Busy transmitting
    AT86RF212_RX_ON                         = 0x06,   //!< Receive mode
    AT86RF212_TRX_OFF                       = 0x08,   //!< TRX module disabled
    AT86RF212_PLL_ON                        = 0x09,   //!< PLL enabled
    AT86RF212_TRX_SLEEP                     = 0x0F,   //!< TRX module sleeping
    AT86RF212_BUSY_RX_AACK                  = 0x11,   //!< RX busy (auto-ack)
    AT86RF212_BUSY_TX_ARET                  = 0x12,   //!< TX busy (auto-retry)
    AT86RF212_RX_AACK_ON                    = 0x16,   //!< RX on (auto-ack)
    AT86RF212_TX_ARET_ON                    = 0x19,   //!< TX on (auto-retry)
    AT86RF212_RX_ON_NOCLK                   = 0x1C,   //!< RX on but no clock enabled
    AT86RF212_RX_AACK_ON_NOCLK              = 0x1D,   //!< RX on (auto-ack) but no clock enabled
    AT86RF212_BUSY_RX_AACK_NOCLK            = 0x1E,   //!< RX busy (auto-ack) but no clock enabled
    AT86RF212_STATE_TRANSITION_IN_PROGRESS  = 0x1F,   //!< Currently changing states
};

/** sub-register TRAC_STATUS in register TRX_STATE */
enum at86rf212_trx_trac_status_e {
    AT86RF212_TRAC_SUCCESS                 = 0,   //!< AACK or ARET successful
    AT86RF212_TRAC_SUCCESS_DATA_PENDING    = 1,   //!< ARET ACK frame received with pending flag set
    AT86RF212_TRAC_SUCCESS_WAIT_FOR_ACK    = 2,   //!< AACK ACK frame will be sent in nexta vailable slot
    AT86RF212_TRAC_CHANNEL_ACCESS_FAILURE  = 3,   //!< Error accessing channel
    AT86RF212_TRAC_NO_ACK                  = 5,   //!< NO ack received
    AT86RF212_TRAC_INVALID                 = 7,   //!< Invalid state
};

/** TAL states */
enum at86rf212_tal_state_e {
    AT86RF212_TAL_IDLE = 0,
    AT86RF212_TAL_TX_AUTO = 1,
    AT86RF212_TAL_TX_END = 2
};

/** CCA Mode ***/
enum at86rf212_cca_mode_e {
    AT86RF212_CCA_MODE_CS_OR_ENERGY = 0,    //!< Carrier sense OR energy above threshold
    AT86RF212_CCA_MODE_ENERGY = 1,          //!< Energy above threshold
    AT86RF212_CCA_MODE_CS = 2,              //!< Carrier sense
    AT86RF212_CCA_MODE_CS_AND_ENERGY = 3    //!< Carrier sense AND energy above threshold
};

/** Modulation mode **/
enum at86rf212_modulation_e {
    ATRF86212_MODULATION_BPSK = 0,          //!< BPSK modulation
    ATRF86212_MODULATION_OQPSK = 2          //!< OQPSK modulation
};

/** OQPSK Data Rate ***/
enum at86rf212_oqpsk_data_rate_e {
    ATRF86212_OQPSK_DATA_RATE_0_100K_1_250K = 0,    //!< Data rate where SUB_MODE 0 100K, 1 250K
    ATRF86212_OQPSK_DATA_RATE_0_200K_1_500K = 1,    //!< Data rate where SUB_MODE 0 200K, 1 500K
    ATRF86212_OQPSK_DATA_RATE_0_400K_1_1000K = 2,   //!< Data rate where SUB_MODE 0 400K, 1 1000K
    ATRF86212_OQPSK_DATA_RATE_0_NA_1_500K = 3       //!< Data rate where SUB_MODE 0 NA, 1 500K
};

enum at86rf212_irq_e {
    AT86RF212_IRQ_NONE          = 0x00,
    AT86RF212_IRQ_0_PLL_LOCK    = 0x01,
    AT86RF212_IRQ_1_PLL_UNLOCK  = 0x02,
    AT86RF212_IRQ_2_RX_START    = 0x04,
    AT86RF212_IRQ_3_TRX_END     = 0x08,
    AT86RF212_IRQ_4_CCA_ED_DONE = 0x10,
    AT86RF212_IRQ_5_AMI         = 0x20,
    AT86RF212_IRQ_6_TRX_UR      = 0x40,
    AT86RF212_IRQ_7_BAT_LOW     = 0x80,
};

enum at86rf212_clkm_rate_e {
    AT86RF212_CLKM_RATE_NONE                    = 0x00,
    AT86RF212_CLKM_RATE_1MHZ                    = 0x01,
    AT86RF212_CLKM_RATE_2MHZ                    = 0x02,
    AT86RF212_CLKM_RATE_4MHZ                    = 0x03,
    AT86RF212_CLKM_RATE_8MHZ                    = 0x04,
    AT86RF212_CLKM_RATE_16MHZ                   = 0x05,
    AT86RF212_CLKM_RATE_250KHZ                  = 0x06,
    AT86RF212_CLKM_RATE_802_15_4_SYMBOL_RATE    = 0x07
};


/** Device defaults ***/
#define AT86RF212_DEFAULT_CHANNEL               1
#define AT86RF212_DEFAULT_CCA_MODE              AT86RF212_CCA_MODE_ENERGY
#define AT86RF212_DEFAULT_MINBE                 3
#define AT86RF212_DEFAULT_MAXBE                 5
#define AT86RF212_DEFAULT_MAX_CSMA_BACKOFFS     4

#define AT86RF212_PLL_LOCK_RETRIES              10
#define AT86RF212_STATE_CHANGE_RETRIES          10


// AT86RF212 object for internal library use
struct at86rf212_s {
    int open;                           //!< Indicates whether the device is open
    struct at86rf212_driver_s* driver;  //!< Driver function object
    void* driver_ctx;                   //!< Driver context
};


#ifdef __cplusplus
}
#endif

#endif
