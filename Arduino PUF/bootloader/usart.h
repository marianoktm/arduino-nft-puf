#ifndef __USART_H__
#define __USART_H__

#include <inttypes.h>

void usart_init(uint32_t ubrr);
void usart_tx_ch(unsigned char data);
void usart_tx_hex(uint8_t val);
void usart_tx_string(char* s);
void usart_tx_buffer(uint8_t* b, uint16_t len);
unsigned char usart_rx_ch(void);
void usart_rx_buffer(uint8_t* b, uint16_t len);
#endif // __USART_H__
