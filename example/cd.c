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
mcp_module(); 

}
void loop(){
 uint16_t val =  mcp_read();
 char buff[10];
  itoa(val,buff,10);
    lcdPrintString("val: ");
    lcdPrintString(buff);
    my_delay(200);
    lcdClear();
 }

void main(){
 setup();
  while (1)
  {
    loop();
  }
  
}
