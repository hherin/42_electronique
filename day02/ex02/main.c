#include "avr/io.h"
#include "util/delay.h" 
#include "avr/interrupt.h"

#define ENABLE_P0 0x0001
#define ENABLE_P1 0x0002
#define ENABLE_P2 0x0004
#define ENABLE_P3 0x0008
#define ENABLE_P4 0x0010
#define ENABLE_ALL (ENABLE_P0 | ENABLE_P1 | ENABLE_P2 | ENABLE_P4)

uint8_t nb = 0;

void sw_handler(int8_t inc)
{
    if (nb)
    {
        PORTB &= ~ENABLE_ALL;       // stop all the leds
    }

    nb += inc;

    int mod = nb % ENABLE_P4;
    // display the result
    if (mod & ENABLE_P3)                // case 4th bit has to be displayed
        PORTB |= (mod & ~ENABLE_P3) | ENABLE_P4; 
    else
        PORTB |= mod;
}

ISR(INT0_vect)
{
    static uint8_t down = 0;
    down++;
    if ((down % 2) != 0)
        sw_handler(1);
    _delay_ms(20);
    EIFR |= (1 << INTF0);
}

ISR(PCINT2_vect)
{
    static uint8_t down = 0;
    down++;
    if ((down % 2) != 0)
        sw_handler(-1);
    _delay_ms(20);
    PCIFR |= (1 << PCIF2);
}


int main(void)
{
    // all led as output
    DDRB |= ENABLE_ALL;
    
    EIMSK |= (1 << INT0);
    EICRA |= (1 << ISC00);

    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT20);
    sei();
    while (1);
}