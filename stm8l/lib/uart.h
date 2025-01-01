#ifndef UART_H
#define UART_H

#include <stdint.h>

void uart_init(void);

void uart_write(uint8_t data);

int uart_write_str(const char *str);

uint8_t uart_read(void);

#endif /* UART_H */
