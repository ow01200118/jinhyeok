#ifndef _DEFINE_H_
#define _DEFINE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>


#define	A_equal_B			-1	// A = B
#define A_large_B			0	// A > B
#define B_large_A			1	// A < B

#define SUCCESS				2
#define FAIL				3

#define zero				10 // A  = 0
#define nonzero				11 // A != 0

#define one					20 // A  = 1
#define nonone				21 // A != 1		


//#define WORD_bit_len		8
//#define WORD_bit_len		32
#define WORD_bit_len		64

#if(WORD_bit_len == 8)
typedef unsigned char WORD;

#elif(WORD_bit_len == 32)
typedef unsigned int	WORD;

#elif(WORD_bit_len == 64)
typedef unsigned __int64	WORD;

#endif

typedef struct Bigint
{
	int sign;				// 1 -> negative( - ), 0 -> positive( + ), The sign of 0 is 0
	int word_len;			// Word length of Bigint
	WORD* a;				// Array of Bigint
}bigint;


#endif
