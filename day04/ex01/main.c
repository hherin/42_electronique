#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>
#include "uart.h"

#define AHT20_ADDR 0x38

void error(void);
void i2c_write(unsigned char data, unsigned char stat_flag);
void print_hex_value(unsigned char c);

static inline void i2c_init(void)
{
    /* enable 2-wire serial interface */
    PRR &= ~(1 << PRTWI);
    TWBR = 0x48; // 72;
    _delay_ms(100);
}

void i2c_start(unsigned char sla_wr, unsigned char stat_flag)
{
    /* send start condition and wait for status code flag */
    TWCR = (1 << TWSTA) | (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1<<TWINT)));
    if ((TWSR & 0xF8) != TW_START)
        error();
    
    /* load SLA_W, transmitter or receiver mode */
    i2c_write(sla_wr, stat_flag);

}

static inline void i2c_stop(void)
{
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void i2c_write(unsigned char data, unsigned char stat_flag)
{
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    if ((TWSR & 0xF8) != stat_flag)
        error();
}

void i2c_read(void)
{
    for (uint8_t i = 0; i < 6; i++)
    {
        TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);

        while (!(TWCR & (1 << TWINT)));
        if ((TWSR & 0xF8) != TW_MR_DATA_ACK)
            error();
        print_hex_value((TWDR)); 
        uart_tx(' ');
    }
    TWCR = (1 << TWINT)| (1 << TWEN);

    while (!(TWCR & (1 << TWINT)));
    if ((TWSR & 0xF8) != TW_MR_DATA_NACK)
        error();
    print_hex_value((TWDR)); 
    uart_printstr("\r\n");


}

void print_hex_value(unsigned char nbr)
{
    if (nbr / 16)
        print_hex_value(nbr / 16);
    uart_tx((nbr % 16) + ((nbr % 16 > 9) ? (65 - 10) : 48));
}


int main(void)
{
    uart_init();
    
    i2c_init();

    while (1)
    {
        i2c_start((AHT20_ADDR << 1), TW_MT_SLA_ACK);
        /* send measurement command */
        i2c_write(0xAC, TW_MT_DATA_ACK);
        i2c_write(0x33, TW_MT_DATA_ACK);
        i2c_write(0x00, TW_MT_DATA_ACK);
        i2c_stop();
        _delay_ms(80);
        i2c_start((AHT20_ADDR << 1) | 0x01, TW_MR_SLA_ACK);
        i2c_read();
        i2c_stop();
        _delay_ms(1000);
    }
}

void error(void)
{
    uart_printstr("error code status: ");
    uart_printnb(TWSR);
    uart_printstr("\r\nStop connexion i2c\r\n");
    i2c_stop();
    return;
}