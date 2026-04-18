#ifndef PIN_MODULE_H
#define PIN_MODULE_H

#define B 'B'
#define D 'D'
#define C 'C'
#define I 'I'
#define O 'O'
#include <avr/io.h>
#include <avr/interrupt.h>

void pin_config(char g,uint8_t n,char m);
void digital_event(char g,uint8_t n,int v);
uint8_t digital_input(char g,uint8_t n);
#endif