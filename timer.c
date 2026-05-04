#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// ISR(ADC_vect){
//     PORTB ^= 0x01;
//     TIFR0 |= (1 << OCF0A);
// }
ISR(TIMER0_COMPA_vect){
    PORTB ^= 0x01;
}
void setup(){
    DDRB |= 0x01;
    // DDRB = 0x01;
    // ADMUX = (1 << REFS0);
    // ADCSRA = (1 << ADEN) | (1 << ADATE) | (1 << ADIE) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
    // ADCSRB = (1 << ADTS0) | (1 << ADTS1);
    // DIDR0 = (1 << ADC0D);

    OCR0A = 100;
    TCNT0 = 0;
    TCCR0A = (1 << WGM01);
    TCCR0B = (1 << CS02)|(1 << CS00);
    TIMSK0 |= (1 << OCIE0A);
    sei();
    //ADCSRA |= (1 << ADSC);
}
void loop(){

}


void main(){
    setup();
    while (1)
    {
        loop();
    }
}