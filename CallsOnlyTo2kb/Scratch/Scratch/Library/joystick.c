/*
 * joystick.c
 *
 * Created: 4/7/2025 11:08:26 PM
 *  Author: sunev
 */
#include "Headers/commons.h"
#include "Headers/joystick.h"

#include <avr/io.h>
#include <util/delay.h>

void adc_init() {
	ADMUX = (1 << REFS0);              // AVcc as reference
	ADCSRA = (1 << ADEN)               // Enable ADC
	| (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // Prescaler 128
}

uint16_t adc_read(uint8_t channel) {
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);  // Select ADC channel
	ADCSRA |= (1 << ADSC);                     // Start conversion
	while (ADCSRA & (1 << ADSC));              // Wait until done
	return ADC;
}

uint16_t smooth_adc(uint8_t channel) {
	uint32_t total = 0;
	for (uint8_t i = 0; i < 4; i++) {
		total += adc_read(channel);
		_delay_ms(2);  // Short delay for stability
	}
	return total / 4;
}

void joystick_init() {
	adc_init();
	DDRD &= ~(1 << SW_PIN);  // SW pin as input
	PORTD |= (1 << SW_PIN);  // Enable pull-up
}