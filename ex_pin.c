#define F_CPU 8000000UL
#include <avr/io>
#include <stdio.h>
#include <stdlib.h>

#include "./include/delay_module.h"
#include "./include/pin_module.h"

void setup(){
    pin_config(B,0,O);
}
void loop(){
    digital_event(B,0,1);
    my_delay(500);
    digital_event(B,0,0);
    my_delay(500);
}

int main(){
    setup();
    while (1)
    {
        loop();
    }
    
}