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

#define AT86RF212_REG_WRITE_FLAG     (0xC0)
#define AT86RF212_REG_READ_FLAG      (0x80)

// AT86RF212 object for internal use
struct at86rf212_s {
    int open;                           //!< Indicates whether the device is open
    struct at86rf212_driver_s* driver;  //!< Driver function object
    void* driver_ctx;                   //!< Driver context
};


#ifdef __cplusplus
}
#endif

#endif
