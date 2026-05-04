#define F_CPU 8000000UL
#include <avr/io.h>
#include "../include/twi_module.h"
#define SSD1306_ADDR 0x3C

uint8_t buffer[1024];

void OLED_command(uint8_t cmd)
{
    TWI_Start();
    TWI_Write(SSD1306_ADDR << 1); // write mode
    TWI_Write(0x00);              // control byte: command
    TWI_Write(cmd);
    TWI_Stop();
}

void OLED_data(uint8_t data)
{
    TWI_Start();
    TWI_Write(SSD1306_ADDR << 1);
    TWI_Write(0x40); // control byte: data
    TWI_Write(data);
    TWI_Stop();
}
void OLED_init(void)
{
    TWI_Init();

    OLED_command(0xAE); // display off
    OLED_command(0xD5);
    OLED_command(0x80); // clock
    OLED_command(0xA8);
    OLED_command(0x3F); // multiplex (64)
    OLED_command(0xD3);
    OLED_command(0x00); // offset
    OLED_command(0x40); // start line

    OLED_command(0x8D);
    OLED_command(0x14); // charge pump
    OLED_command(0x20);
    OLED_command(0x02); // addressing mode (horizontal)

    OLED_command(0xA1); // segment remap
    OLED_command(0xC8); // COM scan direction

    OLED_command(0xDA);
    OLED_command(0x12); // COM pins
    OLED_command(0x81);
    OLED_command(0xCF); // contrast

    OLED_command(0xD9);
    OLED_command(0xF1); // pre-charge
    OLED_command(0xDB);
    OLED_command(0x40); // vcom detect

    OLED_command(0xA4); // display all on resume
    OLED_command(0xA6); // normal display

    OLED_command(0xAF); // display on
}
void OLED_clear(void)
{
    for (uint16_t i = 0; i < 1024; i++)
    {
        OLED_data(0x00);
    }
}

void OLED_drawPixel(uint8_t x, uint8_t y)
{
    if (x >= 128 || y >= 64)
        return;

    uint16_t index = x + (y / 8) * 128;
    buffer[index] |= (1 << (y % 8));
}
void OLED_update(void)
{
    for (uint8_t page = 0; page < 8; page++)
    {
        OLED_command(0xB0 + page); // set page
        OLED_command(0x00);        // lower col
        OLED_command(0x10);        // higher col

        for (uint8_t col = 0; col < 128; col++)
        {
            OLED_data(buffer[page * 128 + col]);
        }
    }
}

void OLED_clear_buffer()
{
    for (int i = 0; i < 1024; i++)
    {
        buffer[i] = 0;
    }
}