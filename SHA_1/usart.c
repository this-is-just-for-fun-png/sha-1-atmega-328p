/*
 * usart.c
 *
 * Created: 30.08.2024 13:54:32
 * Author : peter
 */ 
	#define F_CPU 16000000UL
	#define BAUDRATE 38400UL
	#define Teiler ((F_CPU / (16UL * BAUDRATE)) - 1)

	#include <avr/io.h>
	#include <avr/interrupt.h>
	#include <util/delay.h>

	volatile char command;
	void usart_init(void);
	unsigned char usart_receive(void);
	void usart_put_string(char *string_ptr);
	void usart_put_byte(unsigned char data);


	int main(void){
		DDRB |= (1 << DDB5);		//out
		PORTB &= ~(1 << PORTB5);	//0 setzen
		usart_init();
		sei();
		char my_test[] = "gude ";
		/* Replace with your application code */
		while (1) 
		{
			if(command == 0x01){
				usart_put_string(my_test);
				command = 0x00;
			}
		}
	}

	ISR(USART_RX_vect){
		command = UDR0;
		}
	
	void usart_init(void){
		UCSR0B |= (1 << RXCIE0);	//interrupt rx
		UCSR0B |= (1 << RXEN0);			//enable rx
		UCSR0B |= (1 << TXEN0);							//enable tx
		UCSR0C &= ~((1 << UMSEL01) | (1 << UMSEL00));	//asynchron
		UCSR0C &= ~((1 << UPM01) | (1 << UPM00));		//no parity
		UCSR0C |= ((1 << UCSZ00) | (1 << UCSZ01));		//8 bit transmission
		UBRR0H = (unsigned char) (Teiler >> 8);			//Baudrate
		UBRR0L = (unsigned char) (Teiler);				//Baudrate
	}

	unsigned char usart_receive(void){
		while(!(UCSR0A & (1 << RXC0))){};
		return UDR0;
	}

	void usart_put_byte(unsigned char data){
		while(!(UCSR0A & (1 << UDRE0)));
		UDR0 = data;
	}

	void usart_put_string(char *string_ptr){
		while(*string_ptr != 0x00){
		usart_put_byte(*string_ptr);
		string_ptr++;
		}
	}