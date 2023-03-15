#include "avr/io.h"

int main(void)
{
    DDRB |= (1 << DDB0);  // LED as output
    
    while (1)
    {
        if ((PIND & (1 << PB2)))  // switch state is up 
            PORTB &= ~(1 << PB0);   // turn up LED
        else
            PORTB |= (1 << PB0);    // turn down LED
    }
}