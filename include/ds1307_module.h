#ifndef DS1307_MODULE_H
#define DS1307_MODULE_H

#include <avr/io.h>
#include <avr/interrupt.h>

void rtc_module();
void rtc_setDateTime(uint8_t h, uint8_t m, uint8_t s,uint8_t date, uint8_t month, uint8_t year);
void rtc_getDateTime(uint8_t *h, uint8_t *m, uint8_t *s,uint8_t *date, uint8_t *month, uint8_t *year);
char *monthName(uint8_t m);
#endif