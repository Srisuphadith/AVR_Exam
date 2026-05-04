#ifndef DS1307_MODULE_H
#define DS1307_MODULE_H

#include <avr/io.h>
#include <avr/interrupt.h>

void rtc_module();
void rtc_setDateTime(uint8_t h, uint8_t m, uint8_t s,uint8_t date, uint8_t month, uint8_t year);
void rtc_getDateTime(uint8_t *h, uint8_t *m, uint8_t *s,uint8_t *date, uint8_t *month, uint8_t *year);
char *monthName(uint8_t m);

void TWI_Init();
void TWI_Start();
void TWI_Write(uint8_t data);
uint8_t TWI_Read(uint8_t ack);
void TWI_Stop();


#endif