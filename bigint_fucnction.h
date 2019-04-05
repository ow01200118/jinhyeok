#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include "bigint_define.h"


int bigint_gen(bigint** bignum, int sign, int word_len, WORD *tmp); //Bigint generation function
int bigint_free(bigint** bignum); // Bigint free function


int AddC(bigint** C, bigint* A, bigint* B); // Bigint of addition core function with the same sign
int Add(bigint** C, bigint* A, bigint* B); //  Bigint of addition function


int SubC(bigint** C, bigint* A, bigint* B); // Bigint of subtraction core function with the positve sign
int Sub(bigint** C, bigint*A, bigint* B); // Bigint of subtraction function
int SubC_k(bigint** C, bigint* A, bigint* B); // Bigint of subtraction core function with the positve sign (karatsuba)
int Sub_k(bigint** C, bigint*A, bigint* B); // Bigint of subtraction function (karatsuba)

int sigleword_Mul(WORD* C, WORD A, WORD B); // Single word multiplication function
int MulC(bigint** C, bigint* A, bigint* B); // Bigint of multiplication core function with the positve sign (School-book)
int MulC_K(bigint** C, bigint* A, bigint* B); // Bigint of multiplication core function with the positve sign (Karatsuba)
int K_exp(bigint* A, bigint* B); // Bigint of extension function for Karatsuba
int Mul(bigint** C, bigint* A, bigint* B, char k); // Bigint of multiplication function

int sigleword_Squ(WORD* C, WORD A); // Single word square function
int SquC(bigint** C, bigint* A); // Bigint of square core function with the positve sign (School-book)
int SquC_K(bigint** C, bigint* A); // Bigint of square core function with the positve sign (Karatsuba)
int Squ(bigint** C, bigint* A, char k); // Bigint of square function


void long_div_bit(bigint* A, bigint* B, bigint** quo, bigint** rem); // Bigint of bit-version division function 
void long_div_word(bigint* A, bigint* B, bigint** quo, bigint** rem); // Bigint of word-version division function
void long_divC_word(bigint* A, bigint* B, bigint** quo, bigint** rem); //  Bigint of word-version division function
void long_divCC_word(bigint* A, bigint* B, bigint** quo, bigint** rem); //  Bigint of word-version division function
WORD long_div_2word(bigint* A, bigint* B); //  Bigint of word-version division function

void L_to_R(bigint** C, bigint* A, bigint*  Exp_num); // Bigint of exponentiation
void R_to_L(bigint** C, bigint* A, bigint*  Exp_num); // Bigint of exponentiation
void MontExp(bigint** C, bigint* A, bigint*  Exp_num); // Bigint of exponentiation

int BarretRed(bigint** C, bigint* A, bigint*  Mod_num); // Bigint of reduction
void GCD(bigint** t0t2, bigint* R, bigint* N); // Bigint of reduction
void MontGetR(bigint* N, bigint** R); // Bigint of reduction
void EEA(bigint** R_inv, bigint** N_inv, bigint* R, bigint* N);
void MontRed(bigint** xR, bigint* x, bigint* N, bigint* R, bigint* N_inv); // Bigint of reduction

void MontExpRed(bigint** x_e, bigint* x, bigint* N, bigint* R, bigint* N_inv, bigint* Exp_num);

#endif