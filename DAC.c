#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "./include/oled_module.h"
#include "./include/pin_module.h"


int rotary_value = 0;
uint8_t multipile = 20;
uint8_t st_select = 0;
uint8_t st_a, st_b;
uint8_t origin_y = 32;
uint8_t origin_x = 0;
uint8_t scale_x = 10;
uint8_t scale_y = 10;
uint8_t cnt = 0;


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

ISR(INT0_vect)
{
    int a = !digital_input(D, 2);
    int b = !digital_input(D, 3);

    if (a == b)
    {
        if (st_select == 0)
        {
            rotary_value--;
        }
        else
        {
            multipile--;
        }
    }
    else
    {
        if (st_select == 0)
        {
            rotary_value++;
        }
        else
        {
            multipile++;
        }
    }

    if (rotary_value > 255)
    {
        rotary_value = 255;
    }
    else if (rotary_value < 0)
    {
        rotary_value = 0;
    }
    OCR0A = rotary_value;

    st_a = a;
    st_b = b;
}

ISR(INT1_vect)
{
    int a = !digital_input(D, 2);
    int b = !digital_input(D, 3);

    if (a == b)
    {
        if (st_select == 0)
        {
            rotary_value++;
        }
        else
        {
            multipile++;
        }
    }
    else
    {
        if (st_select == 0)
        {
            rotary_value--;
        }
        else
        {
            multipile--;
        }
    }

    if (rotary_value > 255)
    {
        rotary_value = 255;
    }
    else if (rotary_value < 0)
    {
        rotary_value = 0;
    }
    OCR0A = rotary_value;

    st_a = a;
    st_b = b;
}
ISR(TIMER0_COMPA_vect)
{
    if (!(digital_input(B, 0)))
    {
        PORTB ^= 0x02;
        st_select = ~st_select;
        digital_event(B, 2, 1);
        while (!digital_input(B, 0))
            ;
        digital_event(B, 2, 0);
    }
}
ISR(ADC_vect)
{
    if (cnt < 128)
    {
        int adc_buff = (ADC / 1024.0) * multipile;
        OLED_drawPixel(cnt, origin_y - adc_buff);
        cnt++;
    }
    else
    {
        cnt = 0;
        OLED_update();
        OLED_clear_buffer();
        Oscilloscope_scale();
    }
}
void rotary_encoder_init(){
    pin_config(B, 0, I);
    pin_config(B, 1, O);
    pin_config(D, 3, I);
    pin_config(D, 2, I);
    pin_config(B, 2, O);
    st_a = !digital_input(D, 2);
    st_b = !digital_input(D, 3);

    EICRA |= (1 << ISC01) | (1 << ISC00) | (1 << ISC10) | (1 << ISC11);
    EIMSK |= (1 << INT0) | (1 << INT1);
}
void adc_auto_trigger_init(){
    // ADC
    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADATE) | (1 << ADIE) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
    ADCSRB = (1 << ADTS0) | (1 << ADTS1);
    ADCSRB &= ~(1 << ADTS2);
    DIDR0 = (1 << ADC0D);
}
void counter0_auto_trigger_adc_init(){
    // counter0
    OCR0A = 1;
    TCNT0 = 0;
    TCCR0A = (1 << WGM01);
    TCCR0B = (1 << CS02) | (1 << CS00);
    TIMSK0 |= (1 << OCIE0A);
    sei();
    ADCSRA |= (1 << ADSC);
}
void setup()
{
    rotary_encoder_init();
    adc_auto_trigger_init();
    counter0_auto_trigger_adc_init();
    OLED_init();

    OLED_clear();
    OLED_clear_buffer();

    Oscilloscope_scale();
    OLED_update();
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