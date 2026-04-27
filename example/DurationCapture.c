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

volatile uint16_t startTrick, endTrick;
float duration;
// PB0 เป็น inputCapture เปลี่ยนไม่ได้
ISR(TIMER1_CAPT_vect)
{
    if (TCCR1B & (1 << ICES1))
    {
        startTrick = ICR1;
        // set neg edge
        TCCR1B &= ~(1 << ICES1);
    }
    else
    {
        endTrick = ICR1;
        // set rising edge
        TCCR1B |= (1 << ICES1);
        // cal duration by counting value multiple period
        duration = (endTrick - startTrick) / (float)(F_CPU/256);
        // if(duration > ?){ // <================================ เช็คในนี้นะ ===================

        // }
    }
}
void input_capture()
{
    sei();
    //prescalar 256 and noise canceler
    TCCR1B |= (1 << ICNC1) | 0x04;
    // Input Capture Interrupt Enable
    TIMSK1 |= (1 << ICIE1);
    // trigger interrupt at falling edge
    TCCR1B &= ~(1 << ICES1);
}
void setup()
{
    input_capture();
    pin_config(B,0,I);
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
