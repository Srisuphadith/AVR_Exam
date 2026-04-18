#ifndef DELAY_MODULE_H
#define DELAY_MODULE_H

#include <avr/io.h>
#include <avr/interrupt.h>

extern volatile int runtime;

void delay_module();
void my_delay(int t);

#endif