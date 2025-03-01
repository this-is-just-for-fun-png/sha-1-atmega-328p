/*-------------------------------------------
|Description: ESY Project: SHA1 algorithm written in C for Atmega 328p
|
|
|Function:  This program processes strings through chars received through the USART interface of an Atmega 328p.
|           After entering the command "#!", the encryption begins using the SHA-1 algorithm.
|           After entering the command "#$", the result is read and transmitted back to the PC
|           via the USART interface for output.
|Author: Timo Selzam, s220864@th-ab.de
|First release:
|Changelog: xxx
|License:
--------------------------------------------*/


#define F_CPU 16000000UL
#define BAUDRATE 9600UL

#include "SHA_1.c"

int main()
{
	usart_init();
	sei();
	uint32_t *M;
	uint32_t *W;
	msg = malloc(sizeof(unsigned char));
	while(1)
	{
		if (1000 <= len)
		{
			error(2);
		}
		if (msg[len-2] == '#' && msg[len-1] == '!' && 0 == failsafe)
		{
			blocks = 1 + (len * 8 / 448);
			msg[len-2] ='\0';
			msg[len-1] ='\0';
			M = calloc(16*blocks, sizeof(uint32_t));
			W = calloc(80, sizeof(uint32_t));
			if(NULL == W || NULL == M)
			{
				error(1);
			}
			pad(msg, M);
			process(W, M);
			de_init(W, M);
			len -= 2;
			failsafe = 1;
		}
		if (msg[len-2] == '#' && msg[len-1] == '$' && 1 == failsafe)
		{
			print_char(H, Ho);
			len = 0;
			free(msg);
			msg = calloc((1), sizeof(unsigned char));
			if(NULL == msg)
			{
				error(1);
			}
			failsafe = 0;
		}
	}
}