/*
 * ssd1306.c
 *
 * Created: 4/7/2025 4:39:57 PM
 *  Author: sunev
 */ 
#include "Headers/commons.h"
#include "Headers/ssd1306.h"
#include "Headers/i2c.h"
#include "Headers/font.h"
 
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#define SSD1306_ADDRESS 0x3C  // Default I2C address

void ssd1306_send_command(uint8_t cmd) {
	avr_i2c_start(SSD1306_ADDRESS << 1);
	avr_i2c_write(0x00);  // Control byte: Co = 0, D/C# = 0 => command
	avr_i2c_write(cmd);
	avr_i2c_stop();
}

void ssd1306_send_data(uint8_t data) {
	avr_i2c_start(SSD1306_ADDRESS << 1);
	avr_i2c_write(0x40);  // Control byte: Co = 0, D/C# = 1 => data
	avr_i2c_write(data);
	avr_i2c_stop();
}

void ssd1306_init(void) {
	avr_i2c_init();

	ssd1306_send_command(0xAE); // Display OFF

	ssd1306_send_command(0x20); // Set Memory Addressing Mode
	ssd1306_send_command(0x00); // Horizontal Addressing Mode

	ssd1306_send_command(0xB0); // Page Start
	ssd1306_send_command(0xC8); // COM Output Scan Direction: remapped

	ssd1306_send_command(0x00); // Low column address
	ssd1306_send_command(0x10); // High column address

	ssd1306_send_command(0x40); // Start line address

	ssd1306_send_command(0x81); // Contrast control
	ssd1306_send_command(0x7F);

	ssd1306_send_command(0xA1); // Segment re-map
	ssd1306_send_command(0xA6); // Normal display

	ssd1306_send_command(0xA8); // Multiplex ratio
	ssd1306_send_command(0x3F); // 1/64 duty

	ssd1306_send_command(0xA4); // Display follows RAM

	ssd1306_send_command(0xD3); // Display offset
	ssd1306_send_command(0x00);

	ssd1306_send_command(0xD5); // Display clock
	ssd1306_send_command(0x80);

	ssd1306_send_command(0xD9); // Pre-charge period
	ssd1306_send_command(0xF1);

	ssd1306_send_command(0xDA); // COM pins
	ssd1306_send_command(0x12);

	ssd1306_send_command(0xDB); // VCOM detect
	ssd1306_send_command(0x40);

	ssd1306_send_command(0x8D); // Charge pump
	ssd1306_send_command(0x14);

	ssd1306_send_command(0xAF); // Display ON
}

void ssd1306_clear_screen(void) {
	for (uint8_t page = 0; page < 8; page++) {
		// Set column and page address
		avr_i2c_start(SSD1306_ADDRESS << 1);
		avr_i2c_write(0x00); // Command mode
		avr_i2c_write(0x21); // Set column address
		avr_i2c_write(0);    // Start at 0
		avr_i2c_write(SSD1306_WIDTH - 1); // End at 127
		avr_i2c_write(0x22); // Set page address
		avr_i2c_write(page);
		avr_i2c_write(page);
		avr_i2c_stop();

		// Write 128 bytes of zero for this page
		avr_i2c_start(SSD1306_ADDRESS << 1);
		avr_i2c_write(0x40); // Data mode

		for (uint8_t i = 0; i < SSD1306_WIDTH; i++) {
			avr_i2c_write(0x00);
		}

		avr_i2c_stop();
	}
}

static uint8_t prev_y = 0;
static uint8_t prev_x = 0;

void ssd1306_clear_prev() {
	ssd1306_draw_pixel(prev_x, prev_y, 0);
}

void ssd1306_draw_pixel(uint8_t x, uint8_t y, uint8_t color) {
	if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) return;
	
	avr_i2c_start(SSD1306_ADDRESS << 1);
	avr_i2c_write(0x00); // Command mode
	ssd1306_send_command(0x21);
	ssd1306_send_command(x);
	ssd1306_send_command(x);
	ssd1306_send_command(0x22);
	ssd1306_send_command(y / 8);
	ssd1306_send_command(y / 8);
	avr_i2c_stop();
	
	avr_i2c_start(SSD1306_ADDRESS << 1);
	avr_i2c_write(0x40);  // Data mode
	avr_i2c_write(color << (y % 8));
	avr_i2c_stop();
	
	prev_x = x;
	prev_y = y;
}

uint8_t indexcol = 0;
uint8_t indexpage = 0;

void ssd1306_draw_char(char ch, uint8_t x, uint8_t y) {
	uint8_t offset = x + CHARS_COLS_LENGTH + indexcol;
	uint8_t page = (y / 8);
	if (offset > SSD1306_WIDTH) {
		indexpage++;
		offset = x + CHARS_COLS_LENGTH;
		indexcol = 0;
	}
		
	if (page > 7) { return; }
		
	avr_i2c_start(SSD1306_ADDRESS << 1);
	avr_i2c_write(0x00); // Command mode
	ssd1306_send_command(0x21);
	ssd1306_send_command(x + indexcol);
	ssd1306_send_command(offset);
	ssd1306_send_command(0x22);
	ssd1306_send_command(page + indexpage);
	ssd1306_send_command(page + indexpage);
	avr_i2c_stop();
		
	avr_i2c_start(SSD1306_ADDRESS << 1);
	avr_i2c_write(0x40);
	
	for (uint8_t i = 0; i < 5; i++) {
		uint8_t col = pgm_read_byte(&FONTS[ch - 0x20][i]);
		avr_i2c_write(col);
	}
	
	avr_i2c_stop();
	
	indexcol += CHARS_COLS_LENGTH;
};

void ssd1306_draw_string(char *str, uint8_t x, uint8_t y) {
	if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) return;
	indexcol = 0;
	indexpage = 0;
	uint8_t i = 0;
	
	while (str[i] != '\0') {
		ssd1306_draw_char(str[i++], x, y);
	}
}
