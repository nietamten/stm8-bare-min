#include "spi.h"
#include "stm8l.h"

void SPI_init() {

    //NSS OUT LOW
    PB_DDR |= (1 << 4);
    PB_CR1 |= (1 << 4);
    PB_ODR &= ~(1 << 4);

    //SCK OUT
    PB_DDR |= (1 << 5);
    PB_CR1 |= (1 << 5);
    
    //MOSI OUT
    PB_DDR |= (1 << 6);
    PB_CR1 |= (1 << 6);

    CLK_PCKENR1 = 0xFF; // Enable peripherals
    SPI1_CR2 |= (1<<SPI1_CR2_SSM); // bit 1 SSM=1 Software slave management, enabled
    SPI1_CR2 |= (1<<SPI1_CR2_SSI); // bit 0 SSI=1 Internal slave select, Master mode
    SPI1_CR1 |= (1<<SPI1_CR1_MSTR);  // CR1 bit 2 MSTR = 1, Master configuration.    
    SPI1_CR1 |= (1<<SPI1_CR1_SPE); // SPE, SPI Enable, Peripheral enabled
}

uint8_t SPI_read() {
    SPI_write(0xFF);
    while (!(SPI1_SR & (1 << SPI1_SR_RXNE)));
    return SPI1_DR;
}

void SPI_write(uint8_t data) {
    while ((SPI1_SR & (1 << SPI1_SR_BSY)));
    SPI1_DR = data;  
    while (!(SPI1_SR & (1 << SPI1_SR_TXE)));
}
