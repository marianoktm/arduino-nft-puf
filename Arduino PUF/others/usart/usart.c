#include <avr/io.h>
#include <util/delay.h>

#define BAUD_RATE 		115200UL
#define BAUD_SETTING	((F_CPU/(8UL * BAUD_RATE)) - 1)

void USART_Init(uint32_t ubrr);
void USART_Transmit(unsigned char data);
unsigned char USART_Receive(void);

int main(void)
{
	unsigned char ch = 0;
	USART_Init((unsigned int)BAUD_SETTING);

	for(;;)
	{
		ch = USART_Receive();
		
		if (0 != ch)
		{
			USART_Transmit(ch);
			ch = 0;
		}

	}

	return 0;
}

void USART_Init(uint32_t ubrr)
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

void USART_Transmit(unsigned char data)
{
	/* Wait for empty transmit buffer */
	while (!(UCSR0A & (1 << UDRE0)));
	
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char USART_Receive(void)
{
	/* Wait for data to be received */
	while (!(UCSR0A & (1 << RXC0)));
	
	/* Get and return received data from buffer */
	return UDR0;
}