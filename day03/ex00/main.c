#include <avr/io.h>
#include "util/delay.h" 

#define UART_BAUDRATE 115200

void uart_init()
{
    uint32_t f = ((F_CPU)/ (UART_BAUDRATE * 16));
    /*Set baud rate */
    UBRR0H = (unsigned char)(f >> 8);
    UBRR0L = (unsigned char)f;
    /*Enable transmitter */
    UCSR0B = (1 << TXEN0);
    /* Set frame format: 8data, 2stop bit */
    UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

void uart_tx(char c)
{
    /* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1<<UDRE0)) )
    ;
    /* Put c into buffer, sends the data */
    UDR0 = c;
}

int main(void)
{
    uart_init();
    while(1)
    {
        uart_tx('Z');
        _delay_ms(980);
    }
}