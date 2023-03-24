#ifndef I2C_H
# define I2C_H

#include <avr/io.h>
#define UART_BAUDRATE 115200

static inline void uart_init()
{
    uint32_t f = ((F_CPU)/ (UART_BAUDRATE * 16));
    /*Set baud rate */
    UBRR0H = (unsigned char)(f >> 8);
    UBRR0L = (unsigned char)f;
    /*Enable transmitter */
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    /* Set frame format: 8data, 2stop bit */
    UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

static inline void uart_tx(char c)
{
    /* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1<<UDRE0)) )
    ;

    /* Put c into buffer, sends the data */
    UDR0 = c;
}

static inline void uart_printstr(const char* str)
{
    while (*str)
        uart_tx(*str++);
}

static inline void uart_printnb(unsigned char nb)
{
    if (nb / 10)
        uart_printnb(nb / 10);
    uart_tx((nb % 10) + 48);

}

#endif