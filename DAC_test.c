#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "./include/oled_module.h"
#include "./include/pin_module.h"

int multipile = 20;
uint8_t origin_y = 32;
uint8_t origin_x = 0;
uint8_t scale_x = 10;
uint8_t scale_y = 10;

int rotary_value = 0;
uint8_t conf_select = 0;
uint8_t st_a, st_b;
uint8_t cnt = 0;
int mode = 0;
bool ready = 0;
uint8_t DAC_value = 0;
bool DAC_st;
// ###############################
// #     PROTOTYPE FUNCTION      #
// ###############################
void Oscilloscope_scale();
void rotary_handle(int direction);
void change_mode_and_config(int m);
void component_tester();
void rotary_encoder_init();
void adc_auto_trigger_init();
void counter0_auto_trigger_adc_init();
void pinchange_interrupt_init();
void conter1_enable_ovf_init();
void conter1_enable_OCR1A_init();
// ###############################
// #        UTILS FUNCTION       #
// ###############################
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
void rotary_handle(int direction)
{
    uint8_t a = !(PIND & (1 << PD2));
    uint8_t b = !(PIND & (1 << PD3));
    int rotary_tmp1 = rotary_value;
    if (a == b && direction == 0)
    {
        rotary_value--;
    }
    else if (direction == 0)
    {
        rotary_value++;
    }
    else if (a == b && direction == 1)
    {
        rotary_value++;
    }
    else if (direction == 1)
    {
        rotary_value--;
    }
    int rotary_tmp2 = rotary_value;
    if (rotary_value > 255)
        rotary_value = 255;

    if (rotary_value < 0)
        rotary_value = 0;

    if (conf_select == 0)
    {
        OCR0A = rotary_value;
    }
    else
    {
        multipile += rotary_tmp2 - rotary_tmp1;
    }
    st_a = a;
    st_b = b;
}
int b = 0;
void change_mode_and_config(int m)
{
    // toggle mode
    conf_select = !conf_select;
    // toggle LED status mode
    PORTD = ((uint8_t)conf_select << DDD6);
    // buzzer
    digital_event(D, 5, 1);
    _delay_ms(20);
    digital_event(D, 5, 0);
    if (TIMSK1 & (1 << OCIE1A))
    {
        b = 1;
    }
    conter1_enable_ovf_init();
    while (!digital_input(D, 4))
        ;
    uint16_t t1 = TCNT1;
    if (t1 > 7812)
    {
        mode = m;
        if (m == 1)
        {
            OCR1A = 2;
            conter1_enable_OCR1A_init();
            ADCSRA &= ~((1 << ADATE) | (1 << ADIE));
            ADMUX &= ~(0x0F);
        }
        else
        {
            conter1_enable_ovf_init();
        }
        conf_select = 0;
        PORTD = ((uint8_t)conf_select << DDD6);
    }
    if (b == 1)
    {
        conter1_enable_OCR1A_init();
        b = 0;
    }
    _delay_ms(50);
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
            _delay_ms(4);
        }
        for (int i = 255; i > 0; i--)
        {
            PORTB = (uint8_t)i;
            _delay_ms(4);
        }
        if (mode == 0)
        {
            break;
        }
    }
}
// ###############################
// #        INTERRUPT ZONE       #
// ###############################
// Rotary Encoder 1

ISR(INT0_vect)
{
    rotary_handle(0);
}
ISR(INT1_vect)
{
    rotary_handle(1);
}
ISR(PCINT2_vect)
{
    if (!digital_input(D, 4) && mode == 0)
    {
        change_mode_and_config(1);
    }
    else if (!digital_input(D, 4) && mode == 1)
    {
        change_mode_and_config(0);
    }
}
ISR(TIMER0_COMPA_vect)
{
}
int voltage, current;
ISR(TIMER1_COMPA_vect)
{
    PORTB = DAC_value;

    ADMUX &= ~(0x0F);
    ADCSRA |= (1 << ADIF);
    ADCSRA |= (1 << ADSC);
    while (!(ADCSRA & (1 << ADIF)))
        ;
    ADCSRA |= (1 << ADIF);
    ADCSRA |= (1 << ADSC);
    while (!(ADCSRA & (1 << ADIF)))
        ;
    uint16_t v0 = ADC;
    ADMUX |= (0x01);
    ADCSRA |= (1 << ADIF);
    ADCSRA |= (1 << ADSC);
    while (!(ADCSRA & (1 << ADIF)))
        ;
    ADCSRA |= (1 << ADIF);
    ADCSRA |= (1 << ADSC);
    while (!(ADCSRA & (1 << ADIF)))
        ;
    uint16_t v1 = ADC;

    // voltage = ((DAC_value * 0.0196)*32)/5;
    voltage = (v1-v0) >> 2;
    current = (v0) >> 2;
    OLED_drawPixel(voltage+multipile,32-current*rotary_value);
    if (DAC_value == 255)
    {
        ready = 1;
    }
    DAC_value++;
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
        ready = 1;
    }
}

// ###############################
// #   START INNITIAL ZONE       #
// ###############################
void rotary_encoder_init()
{
    // rotary pin configuration
    // pin_config(D, 4, I); // SW
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
void pinchange_interrupt_init()
{
    PCICR = (1 << PCIE2);
    PCMSK2 = (1 << PCINT20);
}
void conter1_enable_ovf_init()
{
    TCNT1 = 0;
    TCCR1B = (1 << CS12) | (1 << CS10);
}
void conter1_enable_OCR1A_init()
{
    TCNT1 = 0;
    TCCR1B = (1 << CS12) | (1 << CS10) | (1 << WGM12);
    TIMSK1 = (1 << OCIE1A);
}
// ###############################
// #      END INNITIAL ZONE      #
// ###############################

void setup()
{
    // LED
    pin_config(D, 6, O);
    // Buzzer
    pin_config(D, 5, O);
    // set DAC output pin
    DDRB = 0xFF;

    pinchange_interrupt_init();
    rotary_encoder_init();
    adc_auto_trigger_init();
    counter0_auto_trigger_adc_init();
    conter1_enable_ovf_init();
    OLED_init();
    OLED_clear();
    OLED_clear_buffer();

    Oscilloscope_scale();
    OLED_update();
}
void loop()
{
    if (ready && mode == 0)
    {
        cli();
        OLED_update();
        OLED_clear_buffer();
        Oscilloscope_scale();
        ready = 0;
        sei();
    }
    else if (ready && mode == 1)
    {
        cli();
        OLED_update();
        OLED_clear_buffer();
        Oscilloscope_scale();
        ready = 0;
        sei();
    }
}
void main()
{
    setup();
    while (1)
    {
        loop();
    }
}