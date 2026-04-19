#define F_CPU 8000000UL
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

#include "./include/delay_module.h"
#include "./include/pin_module.h"
#include "./include/lcd_module.h"
#include "./include/analog_module.h"

volatile adc_pin = 5;
volatile potentiomenter = 0;
volatile light = 0;
ISR(ADC_vect){
    if (adc_pin == 5) // potentiomenter
    {
        potentiomenter = ADC;
    }else if (adc_pin == 4){ // light
        light = ADC;
    }
}
void setup(){
    lcd_module();
    analog_interrupt_en();
}
void loop(){
    if (adc_pin == 5){
        analog_interrup_mode(5); // potentiomenter
        ADCSRA |= (1 << ADSC);
        adc_pin = 4;
    }else if (adc_pin == 4){
        analog_interrup_mode(4);
        ADCSRA |= (1 << ADSC);
        adc_pin = 5;
    }

    char buff1[10], buff2[10];
    itoa(light,buff1,10);
    lcdPrintString("light: ");
    lcdPrintString(buff1);
    
    lcdLine2();
    itoa(potentiomenter,buff2,10);
    lcdPrintString("Potentio: ");
    lcdPrintString(buff2);
    my_delay(300);
    lcdClear();
    
}

int main(){
    setup();
    while (1)
    {
        loop();
    }
}