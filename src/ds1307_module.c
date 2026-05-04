#define F_CPU 8000000UL
#include <avr/io.h>
#include "../include/twi_module.h"



// ---------------- BCD ----------------

uint8_t decToBcd(uint8_t val)
{
    return ((val / 10) << 4) | (val % 10);
}

uint8_t bcdToDec(uint8_t val)
{
    return ((val >> 4) * 10) + (val & 0x0F);
}
void rtc_module()
{
    TWI_Init();
    // lcdPrintString("1");
    TWI_Start();
    // lcdPrintString("2");
    TWI_Write(0xD0);
    // lcdPrintString("3");
    TWI_Write(0x00);
    // lcdPrintString("4");
    TWI_Write(0x00); // start clock
    // lcdPrintString("5");
    TWI_Stop();
}
void rtc_setDateTime(uint8_t h, uint8_t m, uint8_t s,uint8_t date, uint8_t month, uint8_t year)
{
    TWI_Start();
    TWI_Write(0xD0);
    TWI_Write(0x00);

    TWI_Write(decToBcd(s));
    TWI_Write(decToBcd(m));
    TWI_Write(decToBcd(h));
    TWI_Write(0x01); // day (dummy)
    TWI_Write(decToBcd(date));
    TWI_Write(decToBcd(month));
    TWI_Write(decToBcd(year));

    TWI_Stop();
}

void rtc_getDateTime(uint8_t *h, uint8_t *m, uint8_t *s,uint8_t *date, uint8_t *month, uint8_t *year)
{
    TWI_Start();
    TWI_Write(0xD0);
    TWI_Write(0x00);

    TWI_Start();
    TWI_Write(0xD1);

    *s = bcdToDec(TWI_Read(1));
    *m = bcdToDec(TWI_Read(1));
    *h = bcdToDec(TWI_Read(1));
    TWI_Read(1); // skip day
    *date = bcdToDec(TWI_Read(1));
    *month = bcdToDec(TWI_Read(1));
    *year = bcdToDec(TWI_Read(0));

    TWI_Stop();
}
char *monthName(uint8_t m)
{
    switch (m)
    {
        case 1: return "Jan";
        case 2: return "Feb";
        case 3: return "Mar";
        case 4: return "Apr";
        case 5: return "May";
        case 6: return "Jun";
        case 7: return "Jul";
        case 8: return "Aug";
        case 9: return "Sep";
        case 10: return "Oct";
        case 11: return "Nov";
        case 12: return "Dec";
        default: return "Err";
    }
}

// void setup(){
//     // rtc_setDateTime(13, 07, 0, 25, 3, 26); // date month year h:m
//     rtc_init();
// }
// void loop(){
//     uint8_t h, m, s, d, mo, y;
//     char buffer[20];
//         rtc_getDateTime(&h, &m, &s, &d, &mo, &y);

//         sprintf(buffer, "%02d %s %02d %02d:%02d",
//                 d, monthName(mo), y, h, m);
// }
// int main(void)
// {
//     setup();
//     while (1)
//     {
//         loop();
//     }
// }