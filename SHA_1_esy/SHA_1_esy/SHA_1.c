/*-------------------------------------------
|Description: SHA 1 processing
|
|
|Function:  This code is for setting up the SHA 1 functions
|
|Author: Timo Selzam, s220864@th-ab.de
|First release:
|Changelog: xxx
|License:
--------------------------------------------*/


#include "SHA_1.h"
#include "usart.c"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>



uint32_t f1(uint32_t B, uint32_t C, uint32_t D)
{
	return ((B & C) | (~B & D));
}
uint32_t f2(uint32_t B, uint32_t C, uint32_t D)
{
	return (B ^ C ^ D);
}
uint32_t f3(uint32_t B, uint32_t C, uint32_t D)
{
	return ((B & C) | (B & D) | (C & D));
}
uint32_t f4(uint32_t B, uint32_t C, uint32_t D)
{
	return (B ^ C ^ D);
}
uint32_t rotl(uint32_t X, uint32_t n)
{
	return ((X << n) | (X >> (32-n)));
}

//creating 512bit blocks out of the msg buffer and adding further information
//so that the block will fit to the requirements of the SHA-1 specification
void pad(unsigned char *msg, uint32_t *M)
{
	uint32_t count = 0;
	for(int i = 0; i < 16*blocks; i++)
	{
		for(int k = 0; k < 4; k++)
		{
			if (msg[count] != '\0')
			{
				M[i] |= ((uint32_t)msg[count] << 8*(3-k));
				count++;
			}
		}
	}
	M[count/4] |= (0x80UL << 8*(3 - ((count) % 4)));
	uint64_t bits = count * 8;
	M[16*blocks-2] = (bits >> 32);
	M[16*blocks-1] = bits;
}

//processing 512bit blocks with 160bit input buffer to get 160bit hash
void process(uint32_t *W, uint32_t *M)
{
	uint32_t temp = 0;
	for(uint32_t i = 0; i < blocks; i++)
	{
		uint32_t A = H[0], B = H[1], C = H[2], D = H[3], E = H[4];
		for(uint32_t t = 0; t < 80; t++)
		{
			if(t < 16)
			{
				W[t] = M[t+(16*i)];
			}
			else
			{
				W[t] = rotl(W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16], 1);
			}
			temp = W[t] + K[t/20] + rotl(A, 5) + funktion[t/20](B,C,D) + E;
			E = D;
			D = C;
			C = rotl(B, 30);
			B = A;
			A = temp;
		}
		H[0] += A;
		H[1] += B;
		H[2] += C;
		H[3] += D;
		H[4] += E;
	}
}

//sending final hash by USART to serial monitor & setting H[n] to original value
void print_char(uint32_t *H, const uint32_t *Ho)
{
	for(int i = 0; i < 5; i++)
	{
		for(int k = 0; k < 4; k++)
		{
			usart_put_byte((H[i] >> (8*(3-k))));
		}
	}
	for(unsigned char i = 0; i < 5; i++)
	{
		H[i] = Ho[i];
	}
}

//handling error codes
void error(unsigned int n)
{
	switch(n)
	{
		case 1: usart_put_string("No SRAM available.\n"); break;
		case 2: usart_put_string("MSG too long.\n"); break;
		default: usart_put_string("error xxx\n"); break;
	}
	usart_put_string("restart uC!\n");
	while(1);
}

//free allocated mem and setting ptr to NULL
void de_init(uint32_t *W, uint32_t *M)
{
	free (M);
	M = NULL;
	free (W);
	W = NULL;
}

//receiving msg by USART interrupt from serial monitor
ISR(USART_RX_vect){
	msg = realloc(msg, (len+2)*sizeof(unsigned char));
	if(NULL == msg)
	{
		error(1);
	}
	msg[len] = UDR0;
	len++;
}