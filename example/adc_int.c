#define F_CPU 8000000UL
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

#include "./include/delay_module.h"
#include "./include/pin_module.h"
#include "./include/lcd_module.h"
#include "./include/analog_module.h"
ISR(ADC_vect){
    char buff[10];
    itoa(ADC,buff,10);
    lcdPrintString(buff);
    my_delay(300);
    lcdClear();
}
void setup(){
    lcd_module();
    analog_interrupt_en();
    analog_interrup_mode(5);
    
    // lcdPrintString("Hello world");
    // lcdLine2();
    // lcdPrintString("CPE37");
}
void loop(){
    ADCSRA |= (1 << ADSC);
}

int main(){
    setup();
    while (1)
    {
        loop();
    }
    
}