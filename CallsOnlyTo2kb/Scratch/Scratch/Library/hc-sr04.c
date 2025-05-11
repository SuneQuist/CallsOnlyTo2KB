/*
 * hc_sr04.c
 *
 * Created: 4/7/2025 9:18:41 PM
 *  Author: sunev
 */ 

#include "Headers/commons.h"
#include "Headers/hc-sr04.h"

#include <avr/io.h>
#include <util/delay.h>

#define TRIG_PIN PD2
#define ECHO_PIN PD3

void hcsr04_init() {
	DDRD |= (1 << TRIG_PIN);  // TRIG as output
	DDRD &= ~(1 << ECHO_PIN); // ECHO as input
}

uint16_t hcsr04_read() {
	uint16_t count = 0;

	// Send 10us pulse on TRIG
	PORTD &= ~(1 << TRIG_PIN);
	_delay_us(2);
	PORTD |= (1 << TRIG_PIN);
	_delay_us(10);
	PORTD &= ~(1 << TRIG_PIN);

	// Wait for ECHO to go HIGH
	while (!(PIND & (1 << ECHO_PIN)));

	// Measure how long ECHO stays HIGH
	while ((PIND & (1 << ECHO_PIN))) {
		_delay_us(1);
		count++;
		if (count > 30000) break;  // Timeout safety
	}

	return count / 58;  // Convert to centimeters
}
