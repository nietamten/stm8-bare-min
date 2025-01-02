#include <stdint.h>
#include <stm8l.h>
#include <delay.h>
#include <spi.h>

void main() {
    SPI_init();

    while (1) {
        SPI_write(0x44);
        delay_ms(50);
    }
}
