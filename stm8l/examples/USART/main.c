#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stm8l.h>
#include <uart.h>
#include <delay.h>

/*
 * Redirect stdout to UART
 */
int putchar(int c) {
    uart_write(c);
    return 0;
}

/*
 * Redirect stdin to UART
 */
int getchar() {
    return uart_read();
}


int main() {
    uint8_t counter = 0;
    uart_init();

	char x = uart_read(); //wait for single incoming char

    while (1) {
        printf("Test, %d %c \n", counter++,x);
        delay_ms(500);
    }
}
