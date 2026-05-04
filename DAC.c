#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "./include/ds1307_module.h"
#include "./include/analog_module.h"

#define SSD1306_ADDR 0x3C

uint8_t buffer[1024];
int origin_y = 32;
int origin_x = 0;
int scale_x = 10;
int scale_y = 10;
int cnt = 0;
int precision = 5;
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
void Oscilloscope_scale()
{
    for (int i = 0 + origin_x; i < 128 + origin_x; i++)
    {
        OLED_drawPixel(i + origin_x, origin_y);
        if ((i + origin_x) % scale_x == 0)
        {
            OLED_drawPixel(i + origin_x, origin_y - 1);
            OLED_drawPixel(i + origin_x, origin_y + 1);
            for (int k = origin_y - 1 - scale_y; k > 0; k = k - scale_y)
            {
                OLED_drawPixel(i + origin_x, k);
            }
            for (int k = origin_y + 1 + scale_y; k <= 64; k = k + scale_y)
            {
                OLED_drawPixel(i + origin_x, k);
            }
        }
    }
    // OLED_update();
}
ISR(TIMER0_COMPA_vect){
}
int x = 0;
ISR(ADC_vect)
{
    // PORTB ^= 0x01;
    if (cnt < 128)
    {
            int adc_buff = (ADC / 1024.0) * 20.0;
            OLED_drawPixel(cnt, 32 - adc_buff);
            cnt++;
    }
    else
    {
        x = 0;
        cnt = 0;
        OLED_update();
        OLED_clear_buffer();
        Oscilloscope_scale();
    }
    //ADCSRA |= (1 << ADSC);
}
void setup()
{
    //ADC
    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADATE) | (1 << ADIE) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
    ADCSRB = (1 << ADTS0) | (1 << ADTS1);
    ADCSRB &= ~(1 << ADTS2);
    DIDR0 = (1 << ADC0D);


    //counter0
    OCR0A = 50;
    TCNT0 = 0;
    TCCR0A = (1 << WGM01);
    TCCR0B = (1 << CS02) | (1 << CS00);
    TIMSK0 |= (1 << OCIE0A);
    sei();
    ADCSRA |= (1 << ADSC);

    OLED_init();
    OLED_clear();
    OLED_clear_buffer();
    Oscilloscope_scale();
    OLED_update();

    // DDRB = 0xFF;
}
void loop()
{

}

void main()
{
    setup();
    while (1)
    {
        loop();
    }
}