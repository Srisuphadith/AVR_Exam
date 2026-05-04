#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "./include/pin_module.h"
int rotary_value = 0;
int st_a,st_b;

ISR(INT0_vect){
    if(st_a == 1 && st_b == 1){
        rotary_value++;
    }else{
        rotary_value--;
    }
    st_a = !digital_input(D,2);
    st_b = !digital_input(D,3);
}
ISR(INT1_vect){
    if(st_a == 1 && st_b == 1){
        rotary_value--;
    }else{
        rotary_value++;
    }
    st_a = !digital_input(D,2);
    st_b = !digital_input(D,3);
}
void setup(){
    pin_config(B,0,O);
    pin_config(D,3,I);
    pin_config(D,2,I);
    st_a = !digital_input(D,2);
    st_b = !digital_input(D,3);

    EICRA |= (1 << ISC01)  | (1 << ISC00) | (1 << ISC10) | (1 << ISC11);
    EIMSK |= (1 << INT0) | (1 << INT1);
    sei();

}
void loop(){

    if(rotary_value > 10){
        digital_event(B,0,1);
    }else{
        digital_event(B,0,0);
    }
}

void main(){
    setup();
    while (1)
    {
        loop();
    }
    
}