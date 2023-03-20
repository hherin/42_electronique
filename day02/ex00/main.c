#include "avr/io.h"
#include "util/delay.h" 
#include "avr/interrupt.h"

ISR(INT0_vect) 
{

    PORTB ^= (1 << PB0);
    _delay_ms(20);
    EIFR |= (1 << INTF0);
}

int main(void)
{
    // led D2 as output + pin switch 1 as input
    DDRB |= (1 << PB0) ;
    SREG |= 0x80;
    EICRA |= (1 << ISC00) | (1 << ISC01);
    EIMSK |= (1 << INT0);

    while (1);
}