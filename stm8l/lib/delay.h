#ifndef DELAY_H
#define DELAY_H

#ifndef F_CPU
#warning "F_CPU not defined, using 2MHz by default"
#define F_CPU 2000000UL
#endif

#include <stdint.h>

inline void delay_ms(uint32_t ms) {
    if (ms == 0)
        ms = 1;
    //uint32_t tmp = (((uint32_t)2000000UL / (uint32_t)18 / (uint32_t)1000UL) );
    //tmp *=  ms;

    for (uint32_t i = 0; i < ((F_CPU / 12 / 1000UL) * ms); i++) {
    //for (uint32_t i = 0; i < tmp; i++) {    
        __asm__("nop");
    }
}

#endif /* DELAY_H */
