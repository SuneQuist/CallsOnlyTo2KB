/*
 * joystick.h
 *
 * Created: 4/7/2025 11:07:45 PM
 *  Author: sunev
 */ 
#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdint.h>

void adc_init();
uint16_t adc_read(uint8_t channel);
uint16_t smooth_adc(uint8_t channel);
void joystick_init();

#endif