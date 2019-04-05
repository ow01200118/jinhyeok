#include "bigint_define.h"
#include "bigint_fucnction.h"
#include "bigint_help_function.h"

void L_to_R(bigint** C, bigint* A, bigint*  Exp_num)
{
	int bit_len = 0;
	int i = 0;
	int bit = 0;
	bigint* t = NULL;
	WORD one_init = 0x01;
	bigint_gen(&t, 0, 1, &one_init);

	bit_len = bit_length(Exp_num);

	for (i = (bit_len - 1); i >= 0; i--)
	{
		Squ(C, t, 'k');
		swap_bignum(C, &t);
		bigint_free(C);
		bit = j_bit(Exp_num, i);
		if (bit == 1)
		{
			Mul(C, A, t, 'k');
			swap_bignum(C, &t);
			bigint_free(C);
		}
	
	}

	bigint_gen(C, 0, 1, &one_init);
	swap_bignum(C, &t);
	bigint_free(&t);
}

void R_to_L(bigint** C, bigint* A, bigint*  Exp_num)
{
	bigint* t0 = NULL;
	bigint* t1 = NULL;
	WORD one_init = 0x01;
	int bit_len = 0;
	int bit = 0;
	int i = 0;

	bigint_gen(&t0, 0, 1, &one_init);
	bigint_gen(&t1, A->sign, A->word_len, A->a);

	bit_len = bit_length(Exp_num);

	for (i = (bit_len - 1); i >= 0; i--)
	{
		bit = j_bit(Exp_num, i);
		if (bit == 1)
		{
			Mul(C, t0, t1, 'k');
			swap_bignum(C, &t0);
			bigint_free(C);
		}
		Squ(C, t1, 'k');
		swap_bignum(C, &t1);
		bigint_free(C);
	}

	bigint_gen(C, 0, 1, &one_init);
	swap_bignum(C, &t0);
	bigint_free(&t0);
	bigint_free(&t1);
}

void MontExp(bigint** C, bigint* A, bigint*  Exp_num)
{
	bigint* t0 = NULL;
	bigint* t1 = NULL;
	WORD one_init = 0x01;
	int bit_len = 0;
	int bit = 0;
	int i = 0;

	bigint_gen(&t0, 0, 1, &one_init);
	bigint_gen(&t1, A->sign, A->word_len, A->a);

	bit_len = bit_length(Exp_num);

	for (i = (bit_len - 1); i >= 0; i--)
	{
		bit = j_bit(Exp_num, i);

		if (bit == 1)
		{
			Mul(C, t0, t1, 'k');
			swap_bignum(C, &t0);
			bigint_free(C);

			Squ(C, t1, 'k');
			swap_bignum(C, &t1);
			bigint_free(C);
		}
		else
		{
			Mul(C, t0, t1, 'k');
			swap_bignum(C, &t1);
			bigint_free(C);

			Squ(C, t0, 'k');
			swap_bignum(C, &t0);
			bigint_free(C);
		}
	}

	bigint_gen(C, 0, 1, &one_init);
	swap_bignum(C, &t0);
	bigint_free(&t0);
	bigint_free(&t1);
}