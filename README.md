# libat86rf212

A Dependency Injection based SPI Driver library for the at86rf212 Digital Radio IC from Atmel, designed as an alternative to [ARMmbed/atmel-rf-driver](https://github.com/ARMmbed/atmel-rf-driver) for non-mbed users.
This is a work in progress, see Status for more information.

## Usage
Check out [at86rf212.h](lib/at86rf212/at86rf212.h) for the C language interface, or [at86rf212.hpp](lib/at86rf212/at86rf212.hpp) for the C++ language interface.  

A C++ adaptor is provided in [at86rf212_if.hpp](lib/at86rf212/at86rf212_if.hpp) to adapt between c++ driver objects and the underlying c implementation.  

For C use you will need to implement an SPI driver function with the prototype `int8_t spi_transfer(void* context, uint8_t len, uint8_t *data_out, uint8_t* data_in);` as well as a set of gpio read and write functions.  

For C++ use you can use the above function, or create an object extending `AT86RF212::SpiDriverInterface` that implements the method `int spi_transfer(uint8_t len, uint8_t *data_out, uint8_t* data_in)` as well as a set of gpio read and write functions.  

The above functions should return >= 0 for success, < 0 for failure. For an example (using [USB-Thing](https://github.com/ryankurte/usb-thing) check out the [util](/util/source/main.cpp) and  [bindings](/util/source/usbthing_bindings.c). 

## Status

Early WIP. Initialisation, basic send and receive functionality working, still far from feature complete.

- [X] Register operations
- [X] Initialisation
- [X] Polling
- [X] Simple Send
- [X] Simple Receive
- [ ] Packet building & parsing
- [ ] Auto ACK
- [ ] Auto Retransmit
- [ ] Interrupt Mode
- [ ] DMA support

------

This project is MIT licensed to avoid placing limitations on possible uses. 
If you make improvements, please consider opening a pull request and contributing them back. 
And if you use this in a project, I would love to hear about it (and you can find my email in any of my git commits).  

If you have any questions, comments, or suggestions, feel free to open an issue or a pull request.
