#ifndef ANALOG_MODULE_H
#define ANALOG_MODULE_H

#include <avr/io.h>
#include <avr/interrupt.h>

uint16_t analog_read(uint8_t n);

#endif