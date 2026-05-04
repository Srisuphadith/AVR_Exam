#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>


void analog_module(){
    ADMUX &= ~(0x0F);
    ADCSRA |= (1 << ADEN) | (1 << ADPS0)| (1 << ADPS1) | (1 << ADPS2);
    ADMUX |= (1 << REFS0); 
}
uint16_t analog_read(uint8_t n){
    ADMUX &= ~(0b111);
    ADMUX |= n;
    ADCSRA |= (1 << ADSC);
    while(!(ADCSRA & (1 << ADIF)));
    ADCSRA |= (1 << ADIF);
    return ADC;
}
void analog_interrup_mode(uint8_t n){
    ADMUX &= ~(0x0F);
    ADMUX |= (1 << REFS0) | n;
    ADCSRA |= (1 << ADEN) | (1 << ADPS0)| (1 << ADPS1) | (1 << ADPS2)| (1 << ADSC);
}
void analog_interrupt_en(){
    sei();
    ADCSRA |= (1 << ADIE);
}
void analog_interrupt_dis(){
    ADCSRA &= ~(1 << ADIE);
}

// void setup()
// {

// }
// void loop()
// {

// }

// void main()
// {
//     setup();
//     while (1)
//     {
//         loop();
//     }
// }