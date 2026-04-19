#ifndef ANALOG_MODULE_H
#define ANALOG_MODULE_H

#include <avr/io.h>
#include <avr/interrupt.h>

uint16_t analog_read(uint8_t n);
void analog_interrupt_en();
void analog_interrupt_dis();
void analog_interrup_mode(uint8_t n);
#endif