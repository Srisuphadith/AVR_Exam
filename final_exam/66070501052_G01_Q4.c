// mission 3 ที่เพิ่ม timer
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

// Timer set
int kuy = 1;
int wanted_time = 1;
bool clear = false;
const char *CORRECTED_CODE = "1234";
bool isCorrect = true;

ISR(TIMER1_COMPA_vect) // ไม่สามารถสั่ lcd clear ได้ใน interrupt
{
    if (kuy == wanted_time)
    { // do something
        kuy = 1;
        clear = true;
    }
    else
    {
        kuy++;
    }
}
// ISR(INT0_vect){
//     lcdLine2();
//     lcdPrintString("                ");
//     lcdPrintString("Door is opened");
// }
// end timer set
void setup()
{
    lcd_module();
    pin_config(C, 5, I); // push button
    sei();
    OCR1A = (F_CPU / 1024) * 5; // 1 sec // this is timer <=============
    TIMSK1 |= (1 << OCIE1A);
    // TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
    TCCR1B |= (1 << WGM12);
    // pin_config(D,2,I);
    // //EICRA |= (1 << ISC01);
    // EIMSK |= (1 << INT0);
}
int count = 0;
char str[5];
bool st = false;
void loop()
{
    if (clear)
    {
        // lcdClear();
        lcdHome();
        lcdPrintString("              ");
        lcdHome();
        lcdPrintString("times is up.");
        my_delay(500);
        // lcdClear();
        lcdHome();
        lcdPrintString("              ");
        lcdHome();
        clear = false;
        count = 0;
    }
    // ADCSRA |= (1<<ADSC);
    char str1 = keypad_read();
    if (str1 != 'N')
    {
        if (count == 0)
        {
            TCCR1B |= (1 << CS12) | (1 << CS10);
        }
        my_delay(200); // กันไม่ให้กดแล้วแสดงผลซ้ำเกิน
        lcdPrintChar(str1);
        str[count] = str1;
        count++;
        if (count == 4)
        {
            TCCR1B |= ~((1 << CS12) | (1 << CS10));
            TCNT1 = 0;
            my_delay(500);

            count = 0;
            for (int i = 0; i < 4; i++)
            {
                if (str[i] != CORRECTED_CODE[i])
                {
                    isCorrect = false;
                    break;
                }
            }
            if (isCorrect)
            {
                lcdClear();
                lcdLine2();
                lcdPrintString("Door is opened");
                my_delay(2000);
                lcdClear();
            }
            else
            {
                lcdClear();
                lcdHome();
                lcdPrintString("Password");
                lcdLine2();
                lcdPrintString("is wrong");
                my_delay(2000);
                lcdClear();
                isCorrect = true;
            }

            // my_delay(1000);
            // //lcdClear();
            // lcdHome();
            // lcdPrintString("              ");
            // lcdHome();
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
        lcdHome();
        st = !st;
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