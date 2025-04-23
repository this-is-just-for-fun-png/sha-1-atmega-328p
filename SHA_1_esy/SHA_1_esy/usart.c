/*-------------------------------------------
|Description: USART
|
|
|Function:  This code is for setting up and using the USART API
|
|Author: Timo Selzam, s220864@th-ab.de
|First release:
|Changelog: xxx
|License:
--------------------------------------------*/

#define Teiler ((F_CPU / (16UL * BAUDRATE)) - 1)

#include <avr/io.h>
#include <avr/interrupt.h>

void usart_init(void);
unsigned char usart_receive(void);
void usart_put_string(char *string_ptr);
void usart_put_byte(unsigned char data);

void usart_init(void){
	UCSR0B |= (1 << RXCIE0);						//enable interrupt rx
	UCSR0B |= (1 << RXEN0);							//enable rx
	UCSR0B |= (1 << TXEN0);							//enable tx
	UCSR0C &= ~((1 << UMSEL01) | (1 << UMSEL00));	//asynchron
	UCSR0C &= ~((1 << UPM01) | (1 << UPM00));		//no parity
	UCSR0C |= ((1 << UCSZ00) | (1 << UCSZ01));		//8 bit transmission
	UBRR0H = (unsigned char) (Teiler >> 8);			//Baudrate
	UBRR0L = (unsigned char) (Teiler);				//Baudrate
}
//receiving by polling
unsigned char usart_receive(void){
	while(!(UCSR0A & (1 << RXC0))){};
	return UDR0;
}
//sending when buffer is empty
void usart_put_byte(unsigned char data){
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}
//passing string byte by byte
void usart_put_string(char *string_ptr){
	while(*string_ptr != '\0')
	{
		usart_put_byte(*string_ptr);
		string_ptr++;
	}
}

//test branch