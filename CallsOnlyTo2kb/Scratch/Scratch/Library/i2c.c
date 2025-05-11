/*
 * i2c.c
 *
 * Created: 4/7/2025 4:34:57 PM
 *  Author: sunev
 */ 
#include "Headers/commons.h"
#include "Headers/i2c.h"

#include <avr/io.h>
#include <util/twi.h>

void i2c_init(uint32_t scl_freq)
{
	TWSR0 = 0x00; // Prescaler = 1
	TWBR0 = ((F_CPU / scl_freq) - 16) / 2;
	TWCR0 = (1 << TWEN); // Enable TWI
}

bool i2c_start(uint8_t address)
{
	TWCR0 = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while (!(TWCR0 & (1 << TWINT)));
	
	if ((TWSR0 & 0xF8) != TW_START && (TWSR0 & 0xF8) != TW_REP_START)
	return false;

	TWDR0 = address;
	TWCR0 = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR0 & (1 << TWINT)));

	uint8_t status = TWSR0 & 0xF8;
	return (status == TW_MT_SLA_ACK || status == TW_MR_SLA_ACK);
}

void i2c_stop(void) {
	TWCR0 = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
	while (TWCR0 & (1 << TWSTO)); // Wait for STOP to complete
}

bool i2c_write(uint8_t data) {
	TWDR0 = data;
	TWCR0 = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR0 & (1 << TWINT)));

	return (TWSR0 & 0xF8) == TW_MT_DATA_ACK;
}

uint8_t i2c_read(bool ack) {
	TWCR0 = (1 << TWINT) | (1 << TWEN) | (ack ? (1 << TWEA) : 0);
	while (!(TWCR0 & (1 << TWINT)));

	return TWDR0;
}

bool i2c_write_buffer(uint8_t address, const uint8_t* data, uint8_t length) {
	if (!avr_i2c_start(address)) return false;

	for (uint8_t i = 0; i < length; i++) {
		if (!avr_i2c_write(data[i])) {
			avr_i2c_stop();
			return false;
		}
	}

	avr_i2c_stop();
	return true;
}

bool i2c_read_buffer(uint8_t address, uint8_t* data, uint8_t length) {
	if (!avr_i2c_start(address | 0x01)) return false;

	for (uint8_t i = 0; i < length; i++) {
		data[i] = i2c_read(i < (length - 1));  // ACK for all but last
	}

	avr_i2c_stop();
	return true;
}