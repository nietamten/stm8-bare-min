#include <stdint.h>
#include <stm8l.h>
#include <delay.h>

#define LED_PIN     6

void main() {
    PD_DDR |= (1 << LED_PIN);
    PD_CR1 |= (1 << LED_PIN);

    while (1) {
        PD_ODR ^= (1 << LED_PIN);
        delay_ms(250);
    }
}
