#define F_CPU 8000000UL
#include <avr/io.h>
// #include <avr/interrupt.h>

#include <stdio.h>
#include <stdlib.h>

#include "./include/delay_module.h"
#include "./include/lcd_module.h"
#include "./include/mcp_module.h"
#include "./include/ds1307_module.h"
#include "./include/keypad_module.h"
#include "./include/analog_module.h"
#include "./include/ultrasonic_module.h"

void setup(){
    lcd_module();
    ultrasonic_begin();
}

void loop(){
    char buff[10];
    itoa(distance,buff,10);
    lcdPrintString("distance: ");
    lcdPrintString(buff);
    my_delay(500);
    lcdClear();
}

void main(){
    setup();
    while(1){
        loop();
    }
}