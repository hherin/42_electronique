#include "avr/io.h"
#include "util/delay.h"

int main(void)
{
    DDRB |= (1 << DDB0);  // LED as output
    char down = 0;

    while (1)
    {
        if ( !(PIND  & (1 << PD2)) ){  // switch state is down
            down = 1;
            _delay_ms(1);
        }
        

        if ( (PIND & (1 << PB2)) && down ){
            PORTB ^= (1 << PB0);    // turn down LED
            down = 0;
        }
    }
}