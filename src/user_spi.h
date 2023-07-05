#ifndef USER_SPI_H    /* This is an "include guard" */
#define USER_SPI_H 

#include "pico/stdlib.h"

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi0
#define PIN_RX 	16
#define PIN_CS  17
#define PIN_SCK 18
#define PIN_TX 	19

#define BUF_LEN 0x100

void setup_spi();
void setup_spi_buffers();
void fill_out_buf(uint16_t OUT_DATA);
void send_spi(uint16_t OUT_DATA);
void read_spi();
void sandr_spi(uint16_t OUT_DATA);
void printbuf(uint16_t buf[], size_t len);

#endif /* USER_SPI_H */
