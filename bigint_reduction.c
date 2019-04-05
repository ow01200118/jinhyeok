#include "bigint_define.h"
#include "bigint_fucnction.h"
#include "bigint_help_function.h"


int BarretRed(bigint** C, bigint* A, bigint*  Mod_num)
{
	int N_word_len = 0;
	int A_word_len = 0;
	bigint* R = NULL;
	bigint* Q = NULL;
	bigint* T = NULL;
	bigint* NQ = NULL;
	bigint* tmp = NULL;
	bigint* tmp1 = NULL;

	bigint* rem = NULL;
	bigint* W_2n = NULL;
	WORD one_init = 0x01;
	int ret = 0;


	N_word_len = Mod_num->word_len;
	A_word_len = A->word_len;

	if ((2 * N_word_len) < A_word_len)
	{
		printf("Not Barret reduction condition!!! ( word length of Mod * 2 < word length of Number)\n");
		return FAIL;
	}

	bigint_gen(&W_2n, 0, 1, &one_init);
	left_shift(W_2n, WORD_bit_len * 2 * N_word_len);
	long_div_word(W_2n, Mod_num, &T, &rem);
	


	bigint_gen(&tmp, A->sign, A->word_len, A->a);
	right_shift(tmp, WORD_bit_len * (N_word_len - 1));

	Mul(&Q, T, tmp, 'k');
	right_shift(Q, WORD_bit_len * (N_word_len + 1));
	Mul(&NQ, Mod_num, Q, 'k');
	Sub(&R, A, NQ);

	ret = compare(R, Mod_num);

	while (ret == A_large_B || ret == A_equal_B)
	{
		Sub(&tmp1, R, Mod_num);
		swap_bignum(&tmp1, &R);
		bigint_free(&tmp1);


		ret = compare(R, Mod_num);
	}

	bigint_gen(C, 0, 1, &one_init);
	swap_bignum(C, &R);


	bigint_free(&Q);
	bigint_free(&T);
	bigint_free(&NQ);
	bigint_free(&R);
	bigint_free(&rem);
	bigint_free(&W_2n);
	bigint_free(&tmp);

	return SUCCESS;
}


void EEA(bigint** R_inv, bigint** N_inv, bigint* R, bigint* N)
{
	bigint* t0 = NULL;
	bigint* t0_tmp = NULL;
	bigint* t1 = NULL;
	bigint* t2 = NULL;
	bigint* u0 = NULL;
	bigint* u0_tmp = NULL;
	bigint* v0 = NULL;
	bigint* v0_tmp = NULL;
	bigint* u1 = NULL;
	bigint* v1 = NULL;
	bigint* u2 = NULL;
	bigint* v2 = NULL;

	bigint* t1_tmp = NULL;
	bigint* qu1 = NULL;
	bigint* qv1 = NULL;
	bigint* quo = NULL;
	bigint* rem = NULL;
	WORD zero_init = 0x00;
	WORD one_init = 0x01;
	int ret = 0;

	bigint_gen(&t0, R->sign, R->word_len, R->a);
	bigint_gen(&t1, N->sign, N->word_len, N->a);
	bigint_gen(&u0, 0, 1, &one_init);
	bigint_gen(&v1, 0, 1, &one_init);
	bigint_gen(&u1, 0, 1, &zero_init);
	bigint_gen(&v0, 0, 1, &zero_init);

	ret = is_zero(*t1);

	while (ret != zero)
	{
		bigint_gen(&t2, t0->sign, t0->word_len, t0->a);
		bigint_gen(&t0_tmp, t1->sign, t1->word_len, t1->a);
		swap_bignum(&t0, &t0_tmp);
		bigint_free(&t0_tmp);

		long_div_word(t2, t1, &quo, &t1_tmp);
		swap_bignum(&t1, &t1_tmp);
		bigint_free(&t1_tmp);
		bigint_free(&quo);

		Sub(&t1_tmp, t2, t1);
		long_div_word(t1_tmp, t0, &quo, &rem);
		bigint_free(&rem);
		bigint_free(&t1_tmp);

		bigint_gen(&u2, u0->sign, u0->word_len, u0->a);
		bigint_gen(&v2, v0->sign, v0->word_len, v0->a);

		bigint_gen(&u0_tmp, u1->sign, u1->word_len, u1->a);
		swap_bignum(&u0, &u0_tmp);
		bigint_free(&u0_tmp);

		bigint_gen(&v0_tmp, v1->sign, v1->word_len, v1->a);
		swap_bignum(&v0, &v0_tmp);
		bigint_free(&v0_tmp);
	
		Mul(&qu1, quo, u1, 'k');
		Mul(&qv1, quo, v1, 'k');
		bigint_free(&u1);
		bigint_free(&v1);
		Sub(&u1, u2, qu1);
		Sub(&v1, v2, qv1);
		bigint_free(&qv1);
		bigint_free(&qu1);
		bigint_free(&t2);
		bigint_free(&quo);
		bigint_free(&u2);
		bigint_free(&v2);

		ret = is_zero(*t1);
	}

	bigint_gen(R_inv, u0->sign, u0->word_len, u0->a);
	bigint_gen(N_inv, v0->sign, v0->word_len, v0->a);
	//long_div_word(u0, N, &quo, R_inv);
	//bigint_free(&quo);
	//long_div_word(v0, R, &quo, N_inv);
	//bigint_free(&quo);

	bigint_free(&u0);
	bigint_free(&u1);
	bigint_free(&v0);
	bigint_free(&v1);
	bigint_free(&t0);
	bigint_free(&t1);
}
void GCD(bigint** t0t2, bigint* R, bigint* N)
{
	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* t2 = NULL;
	bigint* t1_tmp = NULL;
	WORD one_init = 0x01;
	int t0_bit = 0;
	int t1_bit = 1;
	int t1_zero = 0;
	int comp = 0;

	bigint_gen(&t0, R->sign, R->word_len, R->a);
	bigint_gen(&t1, N->sign, N->word_len, N->a);
	bigint_gen(&t2, 0, 1, &one_init);

	t0_bit = (int)j_bit(t0, 0);
	t1_bit = (int)j_bit(t1, 0);

	while (t0_bit != 1 && t1_bit != 1)
	{
		right_shift(t0, 1);
		right_shift(t1, 1);
		left_shift(t2, 1);

		t0_bit = (int)j_bit(t0, 0);
		t1_bit = (int)j_bit(t1, 0);
	}

	t1_zero = is_zero(*t1);

	while (t1_zero != zero)
	{
		t0_bit = (int)j_bit(t0, 0);
		while (t0_bit != 1)
		{
			right_shift(t0, 1);
			t0_bit = (int)j_bit(t0, 0);
		}

		t1_bit = (int)j_bit(t1, 0);
		while (t1_bit != 1)
		{
			right_shift(t1, 1);
			t1_bit = (int)j_bit(t1, 0);
		}

		comp = compare(t0, t1);
		if (comp == B_large_A || comp == A_equal_B)
		{
			Sub(&t1_tmp, t1, t0);
			swap_bignum(&t1_tmp, &t1);
			bigint_free(&t1_tmp);
		}
		else
		{
			Sub(&t1_tmp, t0, t1);
			swap_bignum(&t1_tmp, &t0);
			bigint_free(&t1_tmp);
		}
		t1_zero = is_zero(*t1);
	}

	Mul(t0t2, t0, t2, 'k');
}

void MontGetR(bigint* N, bigint** R)
{
	WORD one_init = 0x01;
	bigint* gcd = NULL;
	int ret = 0;

	bigint_gen(R, 0, 1, &one_init);
	left_shift(*R, WORD_bit_len*N->word_len);
	GCD(&gcd, *R, N);
	ret = is_one(*gcd);
	while (ret != one)
	{
		left_shift(*R, WORD_bit_len);
		GCD(&gcd, *R, N);
		ret = is_one(*gcd);
	}

}

void MontRed(bigint** xR, bigint* x, bigint* N, bigint* R, bigint* N_inv)
{
	bigint* N_prime = NULL;
	bigint* m = NULL;
	bigint* t = NULL;
	bigint* t_tmp = NULL;
	bigint* mN = NULL;
	bigint* x_mod_R_tmp = NULL;
	bigint* x_mod_R = NULL;
	bigint* xN_prime = NULL;

	bigint* tmp = NULL;
	bigint* quo = NULL;
	bigint* rem = NULL;
	
	int ret = 0;


	bigint_gen(&N_prime, 1, N_inv->word_len, N_inv->a);

	if (x->word_len < R->word_len)
		bigint_gen(&x_mod_R, x->sign, x->word_len, x->a);
	else
	{

		bigint_gen(&x_mod_R_tmp, x->sign, x->word_len, x->a);
		mod(x_mod_R_tmp, WORD_bit_len * (R->word_len - 1));
		swap_bignum(&x_mod_R, &x_mod_R_tmp);
		bigint_free(&x_mod_R_tmp);

	}

	Mul(&xN_prime, x_mod_R, N_prime, 'k');
	bigint_free(&x_mod_R);
	bigint_free(&N_prime);

	mod(xN_prime, WORD_bit_len * (R->word_len - 1));
	swap_bignum(&xN_prime, &m);
	//long_div_word(xN_prime, R, &quo, &m);
	//bigint_free(&quo);
	bigint_free(&xN_prime);

	Mul(&mN, m, N, 'k');
	Add(&tmp, x, mN);
	right_shift(tmp, WORD_bit_len * (R->word_len - 1));
	swap_bignum(&tmp, &t);
	//long_div_word(tmp, R, &t, &rem);
	//bigint_free(&rem);
	bigint_free(&tmp);
	bigint_free(&m);
	bigint_free(&mN);

	ret = compare(t, N);

	if (ret == A_large_B || ret == A_equal_B)
	{
		Sub(&t_tmp, t, N);
		swap_bignum(&t_tmp, &t);
		bigint_free(&t_tmp);
	}

	bigint_gen(xR, t->sign, t->word_len, t->a);
	bigint_free(&t);
}