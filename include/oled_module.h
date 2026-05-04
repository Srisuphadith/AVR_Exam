#ifndef OLED_MODULE_H
#define OLED_MODULE_H

#include <avr/io.h>
#include <avr/interrupt.h>

void OLED_command(uint8_t cmd);
void OLED_data(uint8_t data);
void OLED_init(void);
void OLED_clear(void);
void OLED_drawPixel(uint8_t x, uint8_t y);
void OLED_update(void);
void OLED_clear_buffer();

#endif