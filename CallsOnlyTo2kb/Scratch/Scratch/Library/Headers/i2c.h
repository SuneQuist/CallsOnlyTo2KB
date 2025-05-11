/*
 * i2c.h
 *
 * Created: 4/7/2025 4:30:17 PM
 *  Author: sunev
 */ 
#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <stdbool.h>

void	avr_i2c_init(void);
bool	avr_i2c_start(uint8_t address);
void	avr_i2c_stop(void);
bool	avr_i2c_write(uint8_t data);

void	i2c_init(uint32_t scl_freq);												
bool	i2c_start(uint8_t address);													// Send START + address
void	i2c_stop(void);																// Send STOP
bool	i2c_write(uint8_t data);													// Write byte
uint8_t i2c_read(bool ack);															// Read byte, ACK = true if expecting more
bool	i2c_write_buffer(uint8_t address, const uint8_t* data, uint8_t length);
bool	i2c_read_buffer(uint8_t address, uint8_t* data, uint8_t length);

#endif