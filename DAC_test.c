#define F_CPU 8000000UL
#define SLICE_WINDOWN_OFFSET 50
#define ORIGIN_Y 32
#define ORIGIN_X 0
#define SCALE_X 10
#define SCALE_Y 10

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdbool.h>
#include "./include/oled_module.h"
#include "./include/pin_module.h"

int multipile_x = 20;
int multipile_y = 20;
int rotary_value = 0;

uint8_t adc_buff;
uint8_t conf_select = 0;
uint8_t st_a, st_b;
uint8_t cnt = 0;
uint8_t mode = 0;
uint8_t DAC_value = 0;

uint16_t voltage, current;

bool ready = false;
bool DAC_st;
bool is_cnt1_int_mode = false;

// ###############################
// #     PROTOTYPE FUNCTION      #
// ###############################
void Oscilloscope_scale();
void rotary_handle(int direction);
void change_mode_and_config(int m);
void rotary_encoder_init();
void adc_auto_trigger_init();
void counter0_auto_trigger_adc_init();
void pinchange_interrupt_init();
void conter1_enable_ovf_init();
void conter1_enable_OCR1A_init();
void component_tester();
// ###############################
// #        UTILS FUNCTION       #
// ###############################
void Oscilloscope_scale()
{
    for (int i = 0 + ORIGIN_X; i < 128 + ORIGIN_X; i++)
    {
        OLED_drawPixel(i + ORIGIN_X, ORIGIN_Y);
        if ((i + ORIGIN_X) % SCALE_X == 0)
        {
            OLED_drawPixel(i + ORIGIN_X, ORIGIN_Y - 1);
            OLED_drawPixel(i + ORIGIN_X, ORIGIN_Y + 1);
            for (int k = ORIGIN_Y - 1 - SCALE_Y; k > 0; k = k - SCALE_Y)
            {
                OLED_drawPixel(i + ORIGIN_X, k);
            }
            for (int k = ORIGIN_Y + 1 + SCALE_Y; k <= 64; k = k + SCALE_Y)
            {
                OLED_drawPixel(i + ORIGIN_X, k);
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
        multipile_y += rotary_tmp2 - rotary_tmp1;
    }
    else
    {
        multipile_x += rotary_tmp2 - rotary_tmp1;
    }
    st_a = a;
    st_b = b;
}
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
        is_cnt1_int_mode = true;
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
            OCR1A = 1;
            conter1_enable_OCR1A_init();
            ADCSRA &= ~((1 << ADATE) | (1 << ADIE));
            ADMUX &= ~(0x0F);
        }
        else
        {
            conter1_enable_ovf_init();
            ADCSRA |= (1 << ADATE) | (1 << ADIE);
            ADMUX &= ~(0x0F);
            ADMUX |= 2;
            ADCSRA |= (1 << ADSC);
            PORTB = 0;
            is_cnt1_int_mode = false;
        }
        conf_select = 0;
        PORTD = ((uint8_t)conf_select << DDD6);
    }
    if (is_cnt1_int_mode)
    {
        conter1_enable_OCR1A_init();
        is_cnt1_int_mode = false;
    }
    _delay_ms(50);
}
void component_tester()
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


    voltage = (((int)v1)) >> 3;
    current = (((int)v0)) >> 4;

    if(cnt >= 127-SLICE_WINDOWN_OFFSET && cnt < 255-SLICE_WINDOWN_OFFSET){
        OLED_drawPixel(voltage + multipile_y,63-current - multipile_x);
    }
    cnt++;
    if (DAC_value == 255)
    {
        cnt = 0;
        ready = 1;
        DAC_value = 0;
    }
    DAC_value++;
}

// ###############################
// #        INTERRUPT ZONE       #
// ###############################
// Rotary Encoder 1
ISR(TIMER0_COMPA_vect)
{
}
ISR(TIMER1_COMPA_vect)
{
    component_tester();
}
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
ISR(ADC_vect)
{
    if (cnt < 128)
    {
        adc_buff = (ADC * multipile_x) >> 10;
        OLED_drawPixel(cnt, ORIGIN_Y - adc_buff);
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
    TCCR1B &= ~(1 << WGM12);
    TCCR1B = (1 << CS12) | (1 << CS10);
    TIMSK1 = 0;
}
void conter1_enable_OCR1A_init()
{
    TCNT1 = 0;
    TCCR1B = (1 << CS12) | (1 << CS10) | (1 << WGM12);
    TIMSK1 = (1 << OCIE1A);
}
// ###############################
// #      MAIN PROGRAM ZONE      #
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
    if (ready)
    {
        cli();
        OLED_update();
        OLED_clear_buffer();
        Oscilloscope_scale();
        ready = false;
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