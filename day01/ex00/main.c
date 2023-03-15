#include "avr/io.h"

int main(void)
{
    DDRB |= (1 << DDB1);
    TCCR1A |= (1 << COM1A0);
    TCCR1B |= (1 << WGM12);
    OCR1A = 31249;
    TCCR1B |= (1 << CS12);
    while(1)
    {}
}