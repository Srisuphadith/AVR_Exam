#ifndef LCD_MODULE_H
#define LCD_MODULE_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

void lcdPrintString(char data[]);
void lcdHome();
void lcdLine2();
void lcdClear();
void lcd_module();
void lcdPrintChar(char data);
#endif