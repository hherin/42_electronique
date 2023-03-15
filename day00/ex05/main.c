#include "avr/io.h"
#include "util/delay.h"

#define ENABLE_T0 0x0001
#define ENABLE_T1 0x0002
#define ENABLE_T2 0x0004
#define ENABLE_T3 0x0008
#define ENABLE_T4 0x0010
#define ENABLE_ALL (ENABLE_T0 | ENABLE_T1 | ENABLE_T2 | ENABLE_T4)


void sw_handler(uint8_t pb, uint8_t pd, int inc)
{
    static char value = 0;
    char down = 0;

    if ( !(PIND & (1 << pd)) ){  // switch state is down
        down = 1;
        _delay_ms(20);
    }

    if ( (PIND & (1 << pb)) && down )
    {
        if (value)
        {
            PORTB &= ~ENABLE_ALL;       // stop all the leds
        }

        value += inc;

        int mod = value % ENABLE_T4;
        // display the result
        if (mod & ENABLE_T3)                // case 4th bit has to be displayed
            PORTB |= (mod & ~ENABLE_T3) | ENABLE_T4; 
        else
            PORTB |= mod;

        down = 0;
    }
}

int main(void)
{
    DDRB |= ENABLE_ALL;

    while (1)
    {
        sw_handler(PD2, PB2, 1);
        sw_handler(PD4, PB4, -1);
    }
}