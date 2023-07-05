#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "user_spi.h"

uint16_t out_buf[BUF_LEN], in_buf[BUF_LEN];

// SPI initialisation. This example will use SPI at 1MHz.
void setup_spi() {
	int baudrate = spi_init(SPI_PORT, 1000*1000);
	spi_set_slave(SPI_PORT, true);
	spi_set_format(SPI_PORT, 16, 0, 0, SPI_MSB_FIRST);
	gpio_set_function(PIN_RX, 	GPIO_FUNC_SPI);
	gpio_set_function(PIN_CS,   GPIO_FUNC_SPI);
	gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
	gpio_set_function(PIN_TX, 	GPIO_FUNC_SPI);
}

// Add placeholder values for the SPI in and out buffers.
void setup_spi_buffers() {
	for (size_t i = 0; i < BUF_LEN; ++i) {
		out_buf[i] = ~i; in_buf[i] = i;
	}
}

// Fill output buffer with data from user
void fill_out_buf(uint16_t OUT_DATA) {
	for (size_t i = 0; i < BUF_LEN; i++) {
		out_buf[i] = OUT_DATA;
	}
}

void send_spi(uint16_t OUT_DATA) {
	spi_write16_blocking(SPI_PORT, out_buf, BUF_LEN);
}

void read_spi () {
	spi_read16_blocking(SPI_PORT, 0x0000, in_buf, BUF_LEN);
}

void sandr_spi (uint16_t OUT_DATA) {
	spi_write16_read16_blocking(SPI_PORT, out_buf, in_buf, BUF_LEN);
	printbuf(in_buf, BUF_LEN);
}

void printbuf(uint16_t buf[], size_t len) {
	int i;
	for (i = 0; i < len; ++i) {
		if (i % 16 == 15)
			printf("%02x\n", buf[i]);
		else
			printf("%02x ", buf[i]);
	}

	// append trailing newline if there isn't one
	if (i % 16) {
		putchar('\n');
	}

}
