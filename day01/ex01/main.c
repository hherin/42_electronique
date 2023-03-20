#include "avr/io.h"

int main(void)
{
    DDRB |= (1 << PB1);
    
    // turn on and off led on match
    TCCR1A |= (1 << COM1A1) | (1 << COM1A0);
    // configure PWM phase and frequency correct
    TCCR1B |= (1 << WGM13);
    //prescaler 256
    TCCR1B |= (1 << CS12);
    // TOP
    ICR1 = F_CPU / (2 * 256);
    //match
    OCR1A = (ICR1 /100 * 95);
    while(1)
    {}
}