#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

#include "./include/delay_module.h"
#include "./include/pin_module.h"

int main(){

    pin_config(B,0,O);
    pin_config(B,1,O);
    while (1)
    {
        digital_event(B,0,1);
        digital_event(B,1,0);
        my_delay_us(1110);
        digital_event(B,0,0);
        digital_event(B,1,1);
        my_delay_us(1110);
    }
    
}