#define F_CPU 8000000UL
#include <avr/io.h>
// #include <avr/interrupt.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "./include/delay_module.h"
#include "./include/lcd_module.h"
#include "./include/pin_module.h"
//#include "./include/mcp_module.h"
// #include "./include/ds1307_module.h"
//#include "./include/keypad_module.h"
//#include "./include/analog_module.h"
//#include "./include/ultrasonic_module.h"
bool st = false;
ISR(INT0_vect){
    st = !st;
    digital_event(B,0,st);
    my_delay(200);
}
void setup()
{
    sei();
    pin_config(D,2,I);
    pin_config(B,0,O);
    //EICRA |= (1 << ISC01);
    EIMSK |= (1 << INT0);
}
void loop()
{


}

void main()
{
    setup();
    while (1)
    {
        loop();
    }
}
