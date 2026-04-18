#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

# define B 'B'
# define D 'D'
# define I 'I'
# define O 'O'

void pin_config(char g,uint8_t n,char m){
    if(g == 'B' & m == 'I'){
        //input mode
        DDRB &= ~(1 << n);
        PORTB |= (1 << n);
    }else if(g == 'B'&& m == 'O'){
        //output
        DDRB |= (1 << n);
        PORTB &= ~(1 << n);
    }else if(g == 'D' && m == 'I'){
        //input mode
        DDRD &= ~(1 << n);
        PORTD |= (1 << n);
    }else if(g == 'D' && m == 'O'){
        //output
        DDRD |= (1 << n);
        PORTD &= ~(1 << n);
    }
}
void digital_event(char g,uint8_t n,int v){
    if(g == 'B' & v == 0){
        //set low
        PORTB &= ~(1 << n);
    }else if(g == 'B' && v == 1){
        //set high
        PORTB |= (1 << n);
    }else if(g == 'D' && v == 0){
        //set low
        PORTD &= ~(1 << n);
    }else if(g == 'D' && v == 1){
        //set high
        PORTD |= (1 << n);
    }
}
uint8_t digital_input(char g,uint8_t n){
    if(g == 'B'){
        if((PINB >> n) & 0x01) return 1;
        return 0;
    }else if(g == 'D'){
        if((PIND >> n) & 0x01) return 1;
        return 0;
    }
}


// void setup(){

// }
// void loop(){

// }

// void main(){
//     setup();
//     while (1)
//     {
//         loop();
//     }
// }