#include "avr/io.h"
#include "util/delay.h" 
#include "avr/interrupt.h"

uint8_t dutycycle = 0;

ISR(TIMER0_COMPA_vect)
{
    static int8_t inc = 1; 
    if (!dutycycle)
        inc = 1;
    else if (dutycycle == 100)
        inc = -1;
    dutycycle += inc;
    OCR1A = (ICR1 * dutycycle) / 100;
}


int main(void)
{
    DDRB |= (1 << PB1);
    // Configure compare match non-inverting mode
    // waveform mode in fast PWM with ICR1 as TOP
    // prescaler = 256
    TCCR1A |= (1 << COM1A1) | (1 << COM1A0) | (1 << WGM11);
    TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS12);
    ICR1 = (F_CPU / 256) / 200;
    OCR1A = (ICR1 * dutycycle) / 100;

    // TO in mode CTC with prescaler of 1024
    // interrupt happen when overflow
    TCCR0A |= (1 << WGM01);
    TCCR0B |= (1 << CS02) | (1 << CS00);
    OCR0A = (F_CPU / (1024)) / 200;
    TIMSK0 |= (1 << OCIE0A);
    sei();

    while (1);
}