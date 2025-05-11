/*
 * uart.c
 *
 * Created: 4/7/2025 11:21:48 PM
 *  Author: sunev
 */ 
#include "Headers/commons.h"
#include "Headers/uart.h"

#include <avr/io.h>

void uart_init(uint16_t baud) {
	uint16_t ubrr = (F_CPU / (16UL * baud)) - 1;

	UBRR0H = (ubrr >> 8);   // Baud rate high byte
	UBRR0L = ubrr;          // Baud rate low byte

	UCSR0B = (1 << TXEN0) | (1 << RXEN0);  // Enable TX and RX
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data, 1 stop bit, no parity
}

void uart_send_char(char c) {
	while (!(UCSR0A & (1 << UDRE0)));  // Wait for empty transmit buffer
	UDR0 = c;                          // Send character
}

void uart_send_string(const char* str) {
	while (*str) {
		uart_send_char(*str++);
	}
}

char uart_receive_char(void) {
	while (!(UCSR0A & (1 << RXC0)));  // Wait for data
	return UDR0;                      // Read received char
}
