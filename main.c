#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdio.h>
#include <stdlib.h>

#include "delay_module.h"
#include "lcd_module.h"
#include "keypad_module.h"
#include "analog_module.h"
#include "ultrasonic_module.h"

void setup()
{
    lcd_module();
    ultrasonic_begin();
    // lcdPrintString("Hiura Mihate");
}
void loop()
{

    // lcdPrintString("Hiura");
    // my_delay(1000);
    // lcdClear();
    // my_delay(10);
    // lcdPrintString("Mihate");
    // my_delay(1000);
    // lcdClear();
    // my_delay(10);
    // char buff = keypad_read();
    // if (buff != 'N')
    // {
    //     lcdClear();
    //     my_delay(70);
    //     lcdPrintChar(buff);
    // }
    char buff[10];
    itoa(distance,buff,10);
    lcdClear();
    lcdPrintString(buff);
    my_delay(100);
}

void main()
{
    setup();
    while (1)
    {
        loop();
    }
}