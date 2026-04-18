#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>



uint16_t analog_read(uint8_t n){
    ADMUX |= (1 << REFS0) | n;
    ADCSRA |= (1 << ADEN) | (1 << ADPS0)| (1 << ADPS1) | (1 << ADPS2) | (1 << ADSC);
    while(!(ADCSRA & (1 << ADIF)));
    return ADC;
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