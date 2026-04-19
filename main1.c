// mission 2  ที่ต่อปุ่มกับ keypad
#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "./include/delay_module.h"
#include "./include/lcd_module.h"
// #include "./include/mcp_module.h"
// #include "./include/ds1307_module.h"
#include "./include/keypad_module.h"
// #include "./include/analog_module.h"
// #include "./include/ultrasonic_module.h"
#include "./include/pin_module.h"
char bufferADC[10];

void setup()
{
    lcd_module();
    delay_module();
    pin_config(C, 5, I); //push button
    // analog_interrupt_en();
    // analog_interrup_mode(5);
    // lcdClear();
    // my_delay(50);
    // lcdPrintString("Hi test lcd");
}
int count = 0;
char str[5];
bool st = false;
void loop()
{
    // ADCSRA |= (1<<ADSC);
    char str1 = keypad_read();
    if (str1 != 'N')
    {
        my_delay(200); // กันไม่ให้กดแล้วแสดงผลซ้ำเกิน
        str[count] = str1;
        count++;
        if (count == 4)
        {
            lcdHome();
            lcdPrintString(str);
            my_delay(1000);
            lcdHome();
            lcdPrintString("    ");
            // lcdClear();
            count = 0;
        }
    }
    if (!digital_input(C, 5))
    {
        st = !st;
        my_delay(100);
    }
    if (st)
    {
        lcdLine2();
        lcdPrintString("Door is opened");
    }
    else
    {
        lcdPrintString("              ");
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