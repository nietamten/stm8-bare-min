#ifndef SPI_H
#define SPI_H

#include <stdint.h>

/*
 * Initialize SPI in MODE1.
 *
 * Pinout:
 * SCK  -> PC5
 * MOSI -> PC6
 * MISO -> PC7
 * CS   -> user defined
 */
void SPI_init();
void SPI_disable();
void SPI_NSS_UP();
void SPI_NSS_DOWN();

uint8_t SPI_write(uint8_t data);

uint8_t SPI_read();

#endif /* SPI_H */

