#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include "delay_module.h"
#include "lcd_module.h"
#include "keypad_module.h"
void setup()
{
    lcd_module();
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
    char buff = keypad_read();
    if (buff != 'N')
    {
        lcdClear();
        my_delay(70);
        lcdPrintChar(buff);
    }
}

void main()
{
    setup();
    while (1)
    {
        loop();
    }
}