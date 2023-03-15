#include <avr/io.h>

int main(void)
{
    // PB0 est output
    DDRB |= (1 << 0);

    while (1)
    {
        long i = 0;
        // occupe le processeur pendant 0.5 sec : 2 instructions dans le code et le
        // processeur execute 1 million d'instruction par seconde
        while(i < 250000)
            i++;
            
        // toggle l'alimentation en courant de PB0
        PORTB ^= (1 << 0);
    }
}