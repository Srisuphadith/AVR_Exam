#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "./include/delay_module.h"
#include "./include/pin_module.h"

bool state = false;
int kuy=1;
int wanted_time = 10
ISR(TIMER1_COMPA_vect)
{
    if (kuy==2){
        state = !state;
        digital_event(B, 0, state);
        kuy=1;
    }else{
        kuy++;
    }
    
}
void setup()
{
    sei();
    pin_config(B, 0, O);
    OCR1A = (F_CPU/1024)*1; //1 sec
    TIMSK1 |= (1 << OCIE1A);
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
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