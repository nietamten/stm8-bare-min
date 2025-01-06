#include <string.h>
#include "stm8l.h"
#include "uart.h"


uint8_t uart_read(void) {
    while (!(USART1_SR & (1 << USART1_SR_RXNE)));
    return USART1_DR;
}


int uart_write_str(const char *str) {
	char i;
	for(i = 0; i < strlen(str); i++) {
		while(!(USART1_SR & (1<<USART1_SR_TXE)));
		USART1_DR = str[i];
	}
	return(i); // Bytes sent
}

void uart_write(uint8_t data){
	while(!(USART1_SR & (1<<USART1_SR_TXE)));
	USART1_DR = data;
}


void uart_init(void) {
//	CLK_DIVR = 0x00; // Set the frequency to 16 MHz
	CLK_PCKENR1 = 0xFF; // Enable peripherals

	PC_DDR = 0x08; // Put TX line on
	PC_CR1 = 0x08;

	USART1_CR2 = (1<<USART1_CR2_TEN)|(1<<USART1_CR2_REN); // Allow TX & RX
	USART1_CR3 &= ~(USART_CR3_STOP1 | USART_CR3_STOP2); // 1 stop bit
	USART1_BRR2 = 0x03; USART1_BRR1 = 0x68; // 9600 baud

}
