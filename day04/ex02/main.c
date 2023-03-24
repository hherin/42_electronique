#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>
#include "uart.h"

#define AHT20_ADDR 0x38

void error(void);
void i2c_write(unsigned char data, unsigned char stat_flag);

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
    uint8_t data[5];

    for (uint8_t i = 0; i < 6; i++)
    {
        TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
        while (!(TWCR & (1 << TWINT)));
        if ((TWSR & 0xF8) != TW_MR_DATA_ACK)
            error();
        if (i > 0) /* Copy humidity and temperature datas in array */
            data[i - 1] = TWDR;
    }

    TWCR = (1 << TWINT)| (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    if ((TWSR & 0xF8) != TW_MR_DATA_NACK)
        error();

    uint32_t tmp = ((((uint32_t)data[2] & 0x0F) << 16) | ((uint32_t)data[3] << 8) | ((uint32_t)data[4]));
    tmp = ((((tmp / 1024) * 20000) / 1024) - 5000);
   

    uart_printstr("Temperature: ");
    uart_printnb(tmp/100);
    uart_tx(',');
    uart_printnb(tmp % 100);
    uart_printstr(".C, Humidity: ");

    tmp = (((uint32_t)data[0] << 12) | ((uint32_t)data[1] << 4) | ((uint32_t)data[2] >> 4));
    tmp = ((tmp / 1024) * 10000) /1024;
    uart_printnb(tmp / 100);
    uart_tx(',');
    uart_printnb(tmp % 100);
    uart_printstr("%\r\n");

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