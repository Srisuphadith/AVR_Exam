#define F_CPU 8000000UL
#include <avr/io.h>
// #include <avr/interrupt.h>

#include <stdio.h>
#include <stdlib.h>

#include "./include/delay_module.h"
#include "./include/lcd_module.h"
#include "./include/mcp_module.h"
#include "./include/ds1307_module.h"
#include "./include/keypad_module.h"
#include "./include/analog_module.h"
#include "./include/ultrasonic_module.h"

void setup()
{
    lcd_module();
    // mcp_begin();
    // ultrasonic_begin();
    // lcdPrintString("Hiura Mihate");
    rtc_module();
    rtc_setDateTime(13, 07, 0, 25, 3, 26); // h : m : s , day , month, year
}
void loop()
{
    /*
    lcdPrintString("Hiura");
    my_delay(1000);
    lcdClear();
    my_delay(10);
    lcdPrintString("Mihate");
    my_delay(1000);
    lcdClear();
    my_delay(10);
    */

    /*
    char buff = keypad_read();
    if (buff != 'N')
    {
        lcdClear();
        my_delay(70);
        lcdPrintChar(buff);
    }
    */

    /*
    char buff[10];
    itoa(distance,buff,10);
    lcdClear();
    lcdPrintString(buff);
    my_delay(100);
    */

    /*
    char buff[20];
    itoa(read_mcp(), buff, 10);
    lcdPrintString(buff);
    my_delay(200);
    lcdClear();
    */

    /*
    uint8_t h, m, s, d, mo, y;
    char buffer1[20];
    char buffer2[20];
    rtc_getDateTime(&h, &m, &s, &d, &mo, &y);
    sprintf(buffer1, "%02d:%02d:%02d", h,m,s);
    lcdPrintString(buffer1);
    lcdLine2();
    sprintf(buffer2, "%02d %s %02d", d,monthName(mo),y);
    lcdPrintString(buffer2);
    my_delay(1000);
    lcdClear();
    */
}

void main()
{
    setup();
    while (1)
    {
        loop();
    }
}