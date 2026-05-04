#ifndef TWI_MODULE_H
#define TWI_MODULE_H

#include <avr/io.h>

void TWI_Init();
void TWI_Start();
void TWI_Write(uint8_t data);
uint8_t TWI_Read(uint8_t ack);
void TWI_Stop();

#endif