#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
//trigger PB3
//echo PB0
volatile uint16_t startTrick, endTrick;
float duration;
float distance;
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
        duration = (endTrick - startTrick) / 31250.0;
        // sound speed,distance and time relation 2s/t = 331+0.6T ,T=25
        // solved equation 17300*t cm
        distance = 17300 * duration;
    }
}
void ultrasonic_begin()
{
    //--------for counting duration echo pin from ultrasonic--------
    // prescalar /1024
    TCCR1B |= 0x04;
    // Input Capture Interrupt Enable
    TIMSK1 |= (1 << ICIE1);
    // trigger interrupt at rising edge
    TCCR1B |= (1 << ICES1);
    sei();

    //--------for send trigger to ultrasonic--------
    // OC2A for trigger pin at ultrasonic
    DDRB |= (1 << PB3);
    // set Fast PWM mode
    TCCR2A |= (1 << WGM20) | (1 << WGM21);
    // Clear OC2A on compare match, set OC2A at BOTTOM,(non-inverting mode).
    TCCR2A |= (1 << COM2A1);
    // set prescalar 256
    TCCR2B |= (1 << CS22) | (1 << CS21);
    // duty cycle 10 %
    OCR2A = (0.1) * 255;
}