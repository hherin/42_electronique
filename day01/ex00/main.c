#include "avr/io.h"

int main(void)
{
    DDRB |= (1 << PB1);
    // toggle OC1A -> LED
    TCCR1A |= (1 << COM1A0);
    // in mode CTC
    TCCR1B |= (1 << WGM12);
    // configure prescaler to 1024
    TCCR1B |= (1 << CS12) | (1 << CS10);
    // match value
    OCR1A = 15625 / 2;
    while(1)
    {}
}