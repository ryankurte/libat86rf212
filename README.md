# at86rf212-library

A Dependency Injection based SPI Driver library for the at86rf212 Digital Radio IC from Atmel.  
This is a work in progress, see Status for more information.

## Usage
Check out [at86rf212.h](lib/include/at86rf212.h) for the C language interface, or [at86rf212.hpp](lib/include/at86rf212.hpp) for the C++ language interface.  

A C++ adaptor WILL BE provided in [at86rf212_if.hpp](lib/include/at86rf212_if.hpp) to adapt between c++ driver objects and the underlying c implementation.  

For C use you will need to implement an SPI driver function with the prototype `int8_t spi_transfer(void* context, uint8_t len, uint8_t *data_out, uint8_t* data_in);`.  

For C++ use you can use the above function, or create an object extending `AT86RF212::SpiDriverInterface` that implements the method `int spi_transfer(uint8_t len, uint8_t *data_out, uint8_t* data_in)`.  

Both of the above functions should return >= 0 for success, < 0 for failure. For an example (using [USB-Thing](https://github.com/ryankurte/usb-thing) check out the [util](/util/source/main.cpp). 

## Status

Early WIP. No (useful) functionality yet.

- [ ] Register operations
- [ ] Initialisation
- [ ] Polling 
- [ ] Interrupt Mode
- [ ] DMA support

------

This project is MIT licensed to avoid placing limitations on possible uses. 
If you make improvements, please consider opening a pull request and contributing them back. 
And if you use this in a project, I would love to hear about it (and you can find my email in any of my git commits).  

If you have any questions, comments, or suggestions, feel free to open an issue or a pull request.
