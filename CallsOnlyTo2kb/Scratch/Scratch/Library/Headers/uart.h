/*
 * uart.h
 *
 * Created: 4/7/2025 11:22:27 PM
 *  Author: sunev
 */ 
#ifndef UART_H
#define UART_H

#include <stdint.h>

void uart_init(uint16_t baud);
void uart_send_char(char c);
void uart_send_string(const char* str);
char uart_receive_char(void);

#endif