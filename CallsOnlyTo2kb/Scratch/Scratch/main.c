/*
 * GccApplication1.c
 *
 * Created: 4/7/2025 4:30:00 PM
 * Author : sunev
 */ 
#include "Library/Headers/commons.h"
#include "Library/Headers/ssd1306.h"
#include "Library/Headers/hc-sr04.h"
#include "Library/Headers/joystick.h"
#include "Library/Headers/uart.h"

#include <stdio.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>

extern void timer1_ctc_ocrnx(void);
extern volatile uint8_t interrupted;

int main(void) {
	timer1_ctc_ocrnx();
	uart_init(9600);

	adc_init();
	joystick_init();
	ssd1306_init();
	ssd1306_clear_screen();
	
	uint16_t x, y;
	
	DDRC &= ~((1 << PC0) | (1 << PC1));  // Ensure ADC pins are inputs
	DDRB |= (1 << PB2) | (1 << PB1);
	
	char tt[150];
	sprintf(tt, "damn gg f fsa fsa ggga gsa fa f a a fa fa faf af asfsa");
	ssd1306_draw_string(tt, 30, 30);

	while (1)
	{
		if (interrupted) {
			interrupted = 0;
			
			y = smooth_adc(0);  // VRx on ADC0
			x = smooth_adc(1);  // VRy on ADC1
			
			ssd1306_clear_prev();
			ssd1306_draw_pixel(((uint32_t)x * 128) / 1028, ((uint32_t)y * 64) / 1028, 1);
		}
	}
}
