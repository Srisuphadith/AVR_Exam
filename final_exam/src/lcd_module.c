#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "../include/delay_module.h"
#include "../include/pin_module.h"

//เเก้ขาตรงนี้นะะะ!!!!!!
//                D4 D5 D6 D7 RS E
char pn_arr[6] = {D, D, D, C, D, B};
uint8_t pn[6] = {0, 1, 2, 4, 4, 6};

void pushData()
{
    digital_event(pn_arr[5], pn[5], 1);
    my_delay_us(20);
    digital_event(pn_arr[5], pn[5], 0);
    my_delay_us(20);
}
void sendData(uint8_t data, bool rs)
{
    if (rs)
    {
        digital_event(pn_arr[4], pn[4], 1);
    }
    for (int i = 0; i < 4; i++)
    {
        digital_event(pn_arr[i], pn[i], (uint8_t)((data >> (4 + i)) & 0x01));
    }
    pushData();
    for (int i = 0; i < 4; i++)
    {
        digital_event(pn_arr[i], pn[i], (uint8_t)((data >> (i)) & 0x01));
    }
    pushData();
    if (rs)
    {
        digital_event(pn_arr[4], pn[4], 0);
    }
}
void lcdPrintString(char data[])
{
    for (int i = 0; data[i] != '\0'; i++)
    {
        sendData(data[i], 1);
    }
}
void lcdPrintChar(char data){
    sendData(data,1);
}
void lcdHome(){
	sendData(0x80,0);
    my_delay(2);
}
void lcdLine2(){
	sendData(0xC0,0);
    my_delay(2);
}
void lcdClear(){
	sendData(0x01,0);
    my_delay(2);
}
void lcd_module()
{
    // D4 D5 D6 D7 RS E
    // delay_module();
    for (int i = 0; i < 6; i++)
    {
        pin_config(pn_arr[i], pn[i], O);
    }
    sendData(0x33, 0);
    sendData(0x32, 0);
    sendData(0x28, 0);
    sendData(0x0C, 0);
    sendData(0x01, 0);
    my_delay(10);
}


// void setup()
// {
//     // run once at start
//     delay_module();
//     lcd_module();

//     lcdPrintString("Hello");
//     lcdLine2();
//     lcdPrintString("Mihate");
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