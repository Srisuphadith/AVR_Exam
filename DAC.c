#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "./include/oled_module.h"
#include "./include/pin_module.h"

int rotary_value = 0;
uint8_t multipile = 20;
uint8_t conf_select = 0;
uint8_t st_a, st_b;
uint8_t origin_y = 32;
uint8_t origin_x = 0;
uint8_t scale_x = 10;
uint8_t scale_y = 10;
uint8_t cnt = 0;
int mode = 0;
uint8_t dac_value = 0;
int dac_st = 0;
int t = 0;
// draw scale
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
}
// Rotary Encoder 1
ISR(INT0_vect)
{
    int a = !digital_input(D, 2);
    int b = !digital_input(D, 3);

    if (a == b)
    {
        if (conf_select == 0)
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
        if (conf_select == 0)
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

// Rotary Encoder 2
ISR(INT1_vect)
{
    int a = !digital_input(D, 2);
    int b = !digital_input(D, 3);

    if (a == b)
    {
        if (conf_select == 0)
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
        if (conf_select == 0)
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
void component_tester()
{
    // disable auto trigger
    // disable adc interrupt
    while (1)
    {
        for (int i = 0; i < 255; i++)
        {
            PORTB = (uint8_t)i;
            _delay_ms(5);
        }
        for (int i = 255; i > 0; i--)
        {
            PORTB = (uint8_t)i;
            _delay_ms(5);
        }
    }
}
// Counter0
ISR(TIMER0_COMPA_vect)
{
    if (!(digital_input(D, 4)) && mode == 0)
    {
        // toggle LED status mode
        PORTD ^= 0x40;
        // toggle mode
        conf_select = !conf_select;
        // on buzzer
        digital_event(D, 5, 1);
        TCNT1 = 0;
        while (!digital_input(D, 4))
            ;
        uint16_t t1 = TCNT1;
        if (t1 > 7812)
        {
            OCR0A = 255;
            mode = 1;
            PORTD &= ~(0x40);
            digital_event(D, 5, 0);
            ADMUX &= ~(0x02);
            ADMUX = 0x01;
        }
        // off buzzer
        digital_event(D, 5, 0);
    }
    else if (mode == 1)
    {
        if (!(digital_input(D, 4)))
        {
            PORTD ^= 0x40;
            conf_select = !conf_select;
            while (!digital_input(D, 4))
                ;
        }
            
    }
}

// Oscilloscope
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
void rotary_encoder_init()
{
    // rotary pin configuration
    pin_config(D, 4, I); // SW
    pin_config(D, 3, I); // CLK
    pin_config(D, 2, I); // DT
    // Save initial state of rotary encoder
    st_a = !digital_input(D, 2);
    st_b = !digital_input(D, 3);
    // INT0 INT1 set rising edge mode
    EICRA |= (1 << ISC01) | (1 << ISC00) | (1 << ISC10) | (1 << ISC11);
    // Enable Interrupt INT0,INT1
    EIMSK |= (1 << INT0) | (1 << INT1);
}
void adc_auto_trigger_init()
{
    // Initial ADC2 for oscilloscope mode
    // set internal refferent voltage and select ADC2
    ADMUX = (1 << REFS0) | 2;
    // Enable adc, set auto trigger mode, Enable Interrupt, prescalar 128
    ADCSRA = (1 << ADEN) | (1 << ADATE) | (1 << ADIE) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
    // select counter 0 compare match OCR0A to auto trigger
    ADCSRB = (1 << ADTS0) | (1 << ADTS1);
    ADCSRB &= ~(1 << ADTS2);
    // Disable digital on ADC2
    DIDR0 = (1 << ADC2D) | (1 << ADC1D) | (1 << ADC0D);
}
void counter0_auto_trigger_adc_init()
{
    // Initial counter0 for adc auto trigger mode with compare match OCR0A
    OCR0A = 1;
    TCNT0 = 0;
    TCCR0A = (1 << WGM01);
    // prescalar 1024
    TCCR0B = (1 << CS02) | (1 << CS00);
    // Enable Interrupt Compare match OCR0A
    TIMSK0 |= (1 << OCIE0A);
    sei();
    // Fist start
    ADCSRA |= (1 << ADSC);
}
void conter1_enable()
{
    TCNT1 = 0;
    TCCR1B = (1 << CS12) | (1 << CS10);
}
void setup()
{
    // LED
    pin_config(D, 6, O);
    // Buzzer
    pin_config(D, 5, O);

    rotary_encoder_init();

    adc_auto_trigger_init();
    counter0_auto_trigger_adc_init();
    conter1_enable();

    OLED_init();
    OLED_clear();
    OLED_clear_buffer();
    Oscilloscope_scale();
    OLED_update();

    // set DAC output pin
    DDRB = 0xFF;
    // PORTB = 65;
}

void loop()
{
    //component_tester();
}

void main()
{
    setup();
    while (1)
    {
        loop();
    }
}