#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "./include/delay_module.h"
#include "./include/lcd_module.h"
#include "./include/pin_module.h"
#include "./include/mcp_module.h"
#include "./include/ds1307_module.h"
#include "./include/keypad_module.h"
#include "./include/analog_module.h"
#include "./include/ultrasonic_module.h"

void setup()
{
    lcd_module();
    //lcdPrintString("Hello world");
}
void loop()
{
    char buff = keypad_read();
    if (buff != 'N')
    {
        lcdClear();
        lcdHome();
        lcdPrintChar(buff);
        my_delay(500);
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
