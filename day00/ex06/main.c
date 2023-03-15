#include "avr/io.h"
#include "util/delay.h"

#define ENABLE_T0 0x0001
#define ENABLE_T1 0x0002
#define ENABLE_T2 0x0004
#define ENABLE_T3 0x0008
#define ENABLE_T4 0x0010
#define ENABLE_ALL (ENABLE_T0 | ENABLE_T1 | ENABLE_T2 | ENABLE_T4)


int main(void)
{
    DDRB |= ENABLE_ALL;
    char value = 1;
    char up = 1;
    while (1)
    {
        if (value == ENABLE_T3)
            up = 0;
        else if (value == ENABLE_T0)
            up = 1;
        
        PORTB &= ~ENABLE_ALL;

        if (value & ENABLE_T3)                // case 4th bit has to be displayed
            PORTB |= (value & ~ENABLE_T3) | ENABLE_T4; 
        else
            PORTB |= value;
        
        _delay_ms(2000);

        if (up)
            value = value << 1;
        else
            value = value >> 1;
       
    }
}