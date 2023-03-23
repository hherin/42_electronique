#include <avr/io.h>
#include "util/delay.h" 
#include "avr/interrupt.h"

#define UART_BAUDRATE 115200
#define PW_FLAG 0x02
#define USR_FLAG 0x01
#define FAIL_FLAG 0x04

char usrname[] = "hherin";
char password[] = "hherin";
char buf[50];

volatile uint8_t flags = USR_FLAG | PW_FLAG;

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
    while ( !( UCSR0A & (1 << UDRE0)) );
    /* Put c into buffer, sends the data */
    UDR0 = (int)c;
}

void uart_printstr(const char* str)
{
    while (*str)
        uart_tx(*str++);
}

uint8_t ft_strncmp(char *c1, char *c2, uint8_t size)
{
    uint8_t i = 0;
    while (i < size && c2[i] && c1[i] && c1[i] == c2[i])
        i++;
    return (uint8_t)(!c1[i] && !c2[i]) ? 0 : FAIL_FLAG;
}

void memclean(void)
{
    uint8_t i = 0;
    while (i < 50)
        buf[i++] = 0;
}


ISR(USART_RX_vect)
{
    unsigned char c = UDR0;
    static uint8_t i = 0;
    
    if (c == '\r')
    {
        if (flags & USR_FLAG)
        {
            flags |= ft_strncmp(buf, usrname, i + 1);
            flags &= ~USR_FLAG;
            uart_printstr("\r\n\tpassword: ");
            memclean();
        }
        else if (flags & PW_FLAG)
        {
            flags |= ft_strncmp(buf, password, i + 1);
            flags &= ~PW_FLAG;
        }
        i = 0;
    }
    else if (c == 127)
    {
        buf[--i] = 0;
        uart_printstr("\b \b");
    }
    else if (i < 50)
    {
        buf[i++] = c;
        if (flags & PW_FLAG)
            c = '*';
        uart_tx(c);
    }
}

int main(void)
{
    uart_init();

    while(1)
    {
        uart_printstr("Enter your login\r\n\tusername: ");

        while (flags & PW_FLAG){}
        
        if (flags & FAIL_FLAG)
        {
            uart_printstr("\r\nBad combinaison username/password\r\n\r\n");
            /* clean username and password input buffers */
            memclean();
        }
        else
        {
            uart_printstr("\r\nHello hherin!\r\nShall we play a game?\r\n");
            return 0;
        }
        flags = USR_FLAG | PW_FLAG;
    }
    return 0;
}