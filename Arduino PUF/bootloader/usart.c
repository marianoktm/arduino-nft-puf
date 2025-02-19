#include <avr/io.h>
#include "usart.h"
#include <stdio.h>
#include <stdlib.h>

void usart_init(uint32_t ubrr)
{
    /** Set baud rate*/
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;

    /** Set double speed mode */
    UCSR0A = (1 << U2X0);

    /** Set frame format: 8data, 2stop bit */
    //UCSR0C = (1 << USBS0) | (3 << UCSZ00);

    /** Set frame format: 8data, 1stop bit */
    UCSR0C = (3 << UCSZ00);

    /** Enable receiver and transmitter */
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
}

void usart_tx_ch(unsigned char data)
{
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1 << UDRE0)))
        ;

    /* Put data into buffer, sends the data */
    UDR0 = data;
}

unsigned char usart_rx_ch(void)
{
    /* Wait for data to be received */
    while (!(UCSR0A & (1 << RXC0)))
        ;

    /* Get and return received data from buffer */
    return UDR0;
}

void usart_tx_hex(uint8_t val)
{
    // extract upper and lower nibbles from input value
    uint8_t upperNibble = (val & 0xF0) >> 4;
    uint8_t lowerNibble = val & 0x0F;

    // convert nibble to its ASCII hex equivalent
    upperNibble += upperNibble > 9 ? 'A' - 10 : '0';
    lowerNibble += lowerNibble > 9 ? 'A' - 10 : '0';

    // print the characters
    usart_tx_ch(upperNibble);
    usart_tx_ch(lowerNibble);
}

void usart_tx_string(char* s)
{
    uint16_t i = 0U;
    while(s[i] != '\0')
    {
        usart_tx_ch(s[i]);
        i++;
    }
}

void usart_tx_buffer(uint8_t* b, uint16_t len)
{
    for (uint16_t i = 0U; i < len; i++)
    {
        usart_tx_hex(b[i]);
    }
    usart_tx_ch('\n');
    usart_tx_ch('\r');
}

void usart_rx_buffer(uint8_t* b, uint16_t len)
{
    char input[len*2];
    for (int i = 0U; i < len*2; i++)
    {
        unsigned char byte=usart_rx_ch();
        sprintf(&input[i], "%c", byte);
    }
    for (int i = 0; i < len; i++) {
        char hex[3] = {input[i*2], input[i*2+1], '\0'};
        b[i] = (uint8_t) strtol(hex, NULL, 16);
    }
}