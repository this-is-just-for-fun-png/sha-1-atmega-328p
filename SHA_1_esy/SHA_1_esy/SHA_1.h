/*-------------------------------------------
|Description: Headerfile for SHA_1.c
|
|Author: Timo Selzam, s220864@th-ab.de
|First release:
|Changelog: xxx
|License:
---------------------------------------------*/

#ifndef SHA_1_H
#define SHA_1_H

typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;
typedef uint32_t (*fct_ptr) (uint32_t, uint32_t, uint32_t);

const uint32_t K[4] = {0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6};
const uint32_t Ho[5] = {0x67452301, 0xefcdab89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};
uint32_t H[5] = {0x67452301, 0xefcdab89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};
uint32_t len = 0;
uint32_t blocks = 0;
uint32_t f1(uint32_t B, uint32_t C, uint32_t D);
uint32_t f2(uint32_t B, uint32_t C, uint32_t D);
uint32_t f3(uint32_t B, uint32_t C, uint32_t D);
uint32_t f4(uint32_t B, uint32_t C, uint32_t D);
uint32_t rotl(uint32_t X, uint32_t n);
char failsafe = 0;

fct_ptr funktion[4] = {f1, f2, f3, f4};

void pad(unsigned char *msg, uint32_t *W);
void process(uint32_t *W, uint32_t *M);
void print_char(uint32_t *H,const uint32_t *Ho);
void error(unsigned int n);
void de_init(uint32_t *W, uint32_t *M);
unsigned char *msg;

#endif