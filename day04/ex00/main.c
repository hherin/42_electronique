#include <avr/io.h>
#include <util/twi.h>
#include "uart.h"

static inline void i2c_init(void)
{
    /* enable 2-wire serial interface */
    PRR &= ~(1 << PRTWI);
    TWBR = 0x48; // 72;
}

void i2c_start(void)
{
    uart_printstr("Start i2c: ");
    /* send start condition and wait for status code flag */
    TWCR |= (1 << TWSTA) | (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1<<TWINT)));
    uart_printnb(TWSR);
    uart_printstr("\r\nconfigure in writing mode: ");
    /* load SLAW */
    TWDR = (0x38 << 1)| 1;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    uart_printnb(TWSR);
}

void i2c_stop(void)
{
    uart_printstr("\r\nStop i2c");
    TWCR |= (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

int main(void)
{
    i2c_init();
    uart_init();
    i2c_start();
    i2c_stop();
    while (1);
}
