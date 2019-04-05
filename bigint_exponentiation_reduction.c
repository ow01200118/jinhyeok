#include "bigint_define.h"
#include "bigint_fucnction.h"
#include "bigint_help_function.h"

void MontExpRed(bigint** x_e, bigint* x, bigint* N, bigint* R, bigint* N_inv, bigint* Exp_num)
{
	bigint* quo = NULL;
	bigint* t = NULL;
	bigint* t_2 = NULL;
	bigint* t_tmp = NULL;
	bigint* pi_x = NULL;
	bigint* t_pi_x = NULL;
	bigint* xR = NULL;
	int bit_len = 0;
	int bit = 0;
	int i = 0;

	long_div_word(R, N, &quo, &t);
	bigint_free(&quo);
	bit_len = bit_length(Exp_num);

	Mul(&xR, x, R, 'k');
	long_div_word(xR, N, &quo, &pi_x);
	bigint_free(&quo);
	bigint_free(&xR);

	
	for (i = (bit_len - 1); i >= 0; i--)
	{
		Squ(&t_2, t, 'k');
		MontRed(&t_tmp, t_2, N, R, N_inv);
		swap_bignum(&t_tmp, &t);
		bigint_free(&t_tmp);
		bigint_free(&t_2);
		bit = j_bit(Exp_num, i);

		if (bit == 1)
		{
			Mul(&t_pi_x, t, pi_x, 'k');
			MontRed(&t_tmp, t_pi_x, N, R, N_inv);
			swap_bignum(&t, &t_tmp);
			bigint_free(&t_tmp);
			bigint_free(&t_pi_x);
		}
		
	}
	
	MontRed(&t_tmp, t, N, R, N_inv);
	bigint_gen(x_e, t_tmp->sign, t_tmp->word_len, t_tmp->a);
	bigint_free(&t_tmp);
	bigint_free(&t);
	bigint_free(&pi_x);
}
