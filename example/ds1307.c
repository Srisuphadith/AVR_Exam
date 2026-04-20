#define F_CPU 8000000UL
#include <avr/io.h>
// #include <avr/interrupt.h>

#include <stdio.h>
#include <stdlib.h>

#include "./include/delay_module.h"
#include "./include/lcd_module.h"
//#include "./include/mcp_module.h"
#include "./include/ds1307_module.h"
//#include "./include/keypad_module.h"
//#include "./include/analog_module.h"
//#include "./include/ultrasonic_module.h"

void setup()
{
    lcd_module();
    rtc_module(); // ds1307 rtc = real time clock

    lcdPrintString("P1");
    rtc_setDateTime(13, 07, 0, 25, 3, 26);
    lcdPrintString("P2");
}
void loop()
{
    uint8_t h; uint8_t m; uint8_t s;uint8_t date; uint8_t month; uint8_t year;
    rtc_getDateTime(&h, &m, &s, &date, &month, &year);
    //lcdPrintString("P3");
    char buff[10];
    itoa(s,buff,10);
    lcdPrintString("Second: ");
    lcdPrintString(buff);
    my_delay(1000);
    lcdClear();

}

void main()
{
    setup();
    while (1)
    {
        loop();
    }
}
