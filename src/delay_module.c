#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

//delay-fn---------------------
volatile int runtime = 0; // 1 = 10 ms
ISR(TIMER0_COMPA_vect){
    runtime++;
}
void delay_module(){
    sei();
    TIMSK0 |= (1 << OCIE0A);
    TCCR0A |= (1 << WGM01);
    OCR0A = 7;
}
void my_delay(int t){
    TCNT0 = 0;
    TCCR0B |= (1 << CS02) | (1 << CS00);
    int start = runtime;
    while((runtime - start) <= t);
    TCCR0B &= ~((1 << CS02) | (1 << CS00));
    runtime = 0;
    
}
//delay-fn---------------------

// void setup(){
//     //run once at start
//     delay_module();
//     DDRB |= (1 << DDB0);
    
// }
// void loop(){
//     //loop
//     PORTB |= (1 << PORTB0);
//     my_delay(1000);
//     PORTB &= ~(1 << PORTB0);
//     my_delay(1000);

// }

// void main(){
//     setup();
//     while (1)
//     {
//         loop();
//     }
// }