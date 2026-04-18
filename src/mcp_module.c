#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

void mcp_begin()
{
    DDRB |= (1 << DDB5);  // SCK -> 7 MCP
    DDRB |= (1 << DDB2);  // SS -> 5 MCP
    DDRB &= ~(1 << DDB4); // MISO -> 6 MCP

    PORTB |= (1 << PB2); // SS High
    SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);
    SPSR |= (1 << SPI2X);
}
uint16_t read_mcp()
{

    PORTB &= ~(1 << PB2);

    SPDR = 0;
    while (!(SPSR & (1 << SPIF)))
        ;
    uint8_t highByte = SPDR;
    // SPSR |= (1 << SPIF);

    SPDR = 0;
    while (!(SPSR & (1 << SPIF)))
        ;
    uint8_t lowByte = SPDR;
    // SPSR |= (1 << SPIF);

    PORTB |= (1 << PB2);
    uint16_t adcValue = (((highByte & 0x1F) << 8) | lowByte) >> 1;
    return adcValue;
}
