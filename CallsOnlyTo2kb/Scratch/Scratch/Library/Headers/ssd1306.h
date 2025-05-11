/*
 * ssd1306.h
 *
 * Created: 4/7/2025 4:39:40 PM
 *  Author: sunev
 */ 
#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64

void ssd1306_init(void);
void ssd1306_send_command(uint8_t cmd);
void ssd1306_send_data(uint8_t data);
void ssd1306_clear_screen(void);
void ssd1306_clear_prev(void);
void ssd1306_draw_pixel(uint8_t x, uint8_t y, uint8_t color);
void ssd1306_draw_char(char ch, uint8_t x, uint8_t y);
void ssd1306_draw_string(char *str, uint8_t x, uint8_t y);

#endif
