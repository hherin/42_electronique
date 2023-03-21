#include <avr/io.h>
#include "util/delay.h" 
#include "avr/interrupt.h"

#define UART_BAUDRATE 115200

void uart_init()
{
    uint32_t f = ((F_CPU)/ (UART_BAUDRATE * 16));
    /*Set baud rate */
    UBRR0H = (unsigned char)(f >> 8);
    UBRR0L = (unsigned char)f;
    /*Enable transmitter */
    UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);
    /* Set frame format: 8data */
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
    sei();
}

void uart_tx(char c)
{
    /* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1 << UDRE0)) )
    ;

    /* Put c into buffer, sends the data */
    UDR0 = c;
}

ISR(USART_RX_vect)
{
    unsigned char c = UDR0;
    
    if (c > 64 && c < 91)
        c += 32;
    else if (c > 96 && c < 123)
        c -= 32;
    uart_tx(c);
}


int main(void)
{
    uart_init();
    while(1);
}