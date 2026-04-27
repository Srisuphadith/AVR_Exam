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
// bool st = false;
// ISR(INT0_vect){
//     st = !st;
//     digital_event(B,0,st);
//     my_delay(200);
// }
void setup()
{
    //pin_config(B,0,O);
    lcd_module();
    //lcdPrintString("Hello world");
    // //ultrasonic_begin();
    // sei();
    // pin_config(D,2,I);
    // pin_config(B,0,O);
    // //EICRA |= (1 << ISC01); // set eage 
    // EIMSK |= (1 << INT0);

}
void loop()
{
    // char buff[10];
    // itoa(distance,buff,10);
    // lcdPrintString(buff);
    // my_delay(200);
    // lcdClear();
    //     if(!(digital_input(D,2))){
    //     st = !st;
    //     my_delay(200);
    // }
    // digital_event(B,0,st);
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
