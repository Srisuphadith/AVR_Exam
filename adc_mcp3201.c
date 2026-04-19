#define F_CPU 8000000UL
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

#include "./include/delay_module.h"
#include "./include/pin_module.h"
#include "./include/lcd_module.h"
#include "./include/analog_module.h"


int potentiomenter = 0;
ISR(ADC_vect){
    potentiomenter = ADC;
}
void setup(){
    lcd_module();
    analog_interrupt_en();
    mcp_module();
}
void loop(){
    analog_interrup_mode(5); // potentiomenter
    uint16_t adc_value = mcp_read();

    char buff1[10];
    itoa(adc_value,buff1,10);
    lcdPrintString("Potentio: ");
    lcdPrintString(buff1);
    my_delay(500);
    lcdClear();
    
    
    
}

int main(){
    setup();
    while (1)
    {
        loop();
    }
}