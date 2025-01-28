#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint32_t H[5] = {0x67452301, 0xefcdab89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};
uint32_t K[4] = {0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6};
typedef uint32_t (*fct_ptr) (uint32_t, uint32_t, uint32_t);


uint32_t len = 0;
uint32_t blocks = 0;
uint32_t pos = 0;
uint64_t bits = 0;
uint32_t count = 0;
uint32_t f1(uint32_t B, uint32_t C, uint32_t D);
uint32_t f2(uint32_t B, uint32_t C, uint32_t D);
uint32_t f3(uint32_t B, uint32_t C, uint32_t D);
uint32_t f4(uint32_t B, uint32_t C, uint32_t D);
uint32_t rotl(uint32_t X, uint32_t n);
fct_ptr funktion[4] = {f1, f2, f3, f4};

void pad(unsigned char *msg, uint32_t *W);
void process(uint32_t *W, uint32_t *M);
void print(uint32_t *H);

int main()
{
  
   unsigned char msg[120] = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";

   for(int i = 0; msg[i] != '\0'; i++)
   {
     len = i+1;
   }
   blocks = 1 + (len * 8 / 448);
   pos = 3 - len  % 3;
   bits = len * 8;
   printf("%d\n", blocks);
   uint32_t *M = calloc(80*blocks, sizeof(uint32_t));
   uint32_t *W = calloc(16*blocks, sizeof(uint32_t));
   pad(msg, W);
   process(W, M);
   print(H);
   
   
}

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
void pad(unsigned char *msg, uint32_t *W)
{
     for(int i = 0; i < 16*blocks; i++)
   {
     if(msg[count] != '\0')
     {
       for(int k = 0; k < 4; k++)
       {
         W[i] |= (msg[count] << (8*(3 - count)));
         count++;
       }
      }
      else
      {
        break;
      }
   }
   W[len/4] |= (0x80 << 8*(3 - ((len) % 4)));
   W[16*blocks-2] = (bits >> 32);
   W[16*blocks-1] = bits;
   for(int i = 0; i < 16*blocks; i++)
   {
     printf("W[%02d] 0x%08x\n",i, W[i]);
   }
}
void process(uint32_t *W, uint32_t *M)
{
  
  uint temp = 0;
  for(uint32_t i = 0; i < blocks; i++)
  {
    uint32_t A = H[0], B = H[1], C = H[2], D = H[3], E = H[4];
    for(uint32_t t = 0; t < 80; t++)
     {
     if(t < 16)
     {
       M[t] = W[t+(16*i)];
     }
     else
     {
       M[t] = rotl(M[t-3] ^ M[t-8] ^ M[t-14] ^ M[t-16], 1);
     }
     temp = rotl(A, 5) + funktion[t/20](B,C,D) + E + M[t] + K[t/20];
     E = D;
     D = C; 
     C = rotl(B, 30); 
     B = A;
     A = temp;
     printf("t = %03d  %08x %08x %08x %08x %08x\n", t, A, B, C, D, E);
     }
     H[0] += A;
     H[1] += B;
     H[2] += C;
     H[3] += D;
     H[4] += E;
     printf("\nH0 = %08x   H1 = %08x   H2 = %08x   H3 = %08x   H4 = %08x\n", H[0], H[1], H[2], H[3], H[4]);
  }
}
void print(uint32_t *H)
{
  char temp = 0;
  for(int i = 0; i < 5; i++)
  {
    printf("H[%d] = ", i);
    for(int k = 0; k < 32; k++)
    {
      if(H[i] & (1 << (31 - k)))
      {
        printf("1");
      }
      else
      {
        printf("0");
      }
      if((k+1)%4 == 0)
      {
      printf(" ");
      }
    }

    printf("\n");
  }
}