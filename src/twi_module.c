#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

void TWI_Init()
{
    TWBR = 2;
    TWSR = 0x00;
    TWCR = (1 << TWEN);
}

void TWI_Start()
{
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    // lcdPrintString("TWI_start");
    while (!(TWCR & (1 << TWINT)))
        ;
    // lcdPrintString("1111");
}

void TWI_Write(uint8_t data)
{
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)))
        ;
}

uint8_t TWI_Read(uint8_t ack)
{
    if (ack)
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    else
        TWCR = (1 << TWINT) | (1 << TWEN);

    while (!(TWCR & (1 << TWINT)))
        ;
    return TWDR;
}

void TWI_Stop()
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}