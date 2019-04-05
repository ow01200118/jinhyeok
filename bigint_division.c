#include "bigint_define.h"
#include "bigint_fucnction.h"
#include "bigint_help_function.h"


void long_div_bit(bigint* A, bigint* B, bigint** quo, bigint** rem)
{
	WORD zero_init = 0x00;
	WORD one_init = 0x01;
	WORD bit = 0;


	bigint* R2 = NULL;
	bigint* R3 = NULL;
	bigint* Q2 = NULL;
	bigint* Q3 = NULL;
	bigint* M = NULL;
	bigint* num1 = NULL;

	WORD k_bit = 0;
	int ret = 0;
	int bit_len = 0;
	int i = 0;

	bit_len = bit_length(A); // A의 비트 길이를 구함

	bigint_gen(quo, 0, 1, &zero_init); // 몫을 0으로 초기화
	bigint_gen(rem, 0, 1, &zero_init); // 나머지를 0으로 초기화

	for (i = bit_len - 1; i >= 0; i--)
	{

		left_shift(*rem, 1); // rem을 왼쪽으로 1비트 이동한 후 최하위 비트에 A의 i번째 비트 저장
		bit = j_bit(A, i);


		(*rem)->a[0] = ((*rem)->a[0] ^ bit);

		ret = compare(*rem, B);
		if (ret == A_large_B || ret == A_equal_B)
		{
			bigint_gen(&num1, 0, 1, &one_init);
			left_shift(num1, i); // 2^i 계산
			Add(&Q2, *quo, num1); // Q2 = quo + 2^j
			swap_bignum(&Q2, quo); // Q2 <-> quo
			bigint_free(&Q2);

			Sub(&R2, *rem, B); //R2 = rem - B;
			swap_bignum(&R2, rem); // R2 <-> rem
			bigint_free(&R2);
			bigint_free(&num1);
		}

	}

	if (A->sign == 1) // 나눠지는 수가 음수인 경우
	{
		bigint_gen(&num1, 0, 1, &one_init);
		Add(&Q3, *quo, num1);
		swap_bignum(&Q3, quo);
		bigint_free(&Q3);
		(*quo)->sign = 1;
		Mul(&M, B, *quo, 'k');
		Sub(&R3, A, M);
		swap_bignum(&R3, rem);
		bigint_free(&R3);
		bigint_free(&M);
	}
}









void long_div_word(bigint* A, bigint* B, bigint** quo, bigint** rem)
{
	bigint* R = NULL;
	bigint* Q_tmp = NULL;
	bigint* R_tmp = NULL;
	WORD zero_init = 0;
	int i = 0;
	int j = 0;
	int ret = 0;

	bigint* num1 = NULL;
	WORD one_init = 0x01;
	bigint* Q3 = NULL;
	bigint* R3 = NULL;
	bigint* M = NULL;

	bigint_gen(&R, 0, 1, &zero_init);
	bigint_gen(quo, 0, 1, &zero_init);
	bigint_gen(rem, 0, 1, &zero_init);

	for (i = A->word_len - 1; i >= 0; i--)
	{
		left_shift(R, WORD_bit_len);
		ret = is_zero(*R);
		if (ret == zero)
		{
			R->word_len = 1;
			R->a = (WORD *)realloc(R->a, sizeof(WORD));
		}
		memcpy(R->a, &(A->a[i]), sizeof(WORD));

		ret = is_zero(**quo);
		if (ret == zero)
		{
			(*quo)->word_len = 1;
			(*quo)->a = (WORD *)realloc((*quo)->a, sizeof(WORD));
		}


		ret = compare(R, B);

		if (ret != 1)
		{
			left_shift(*quo, WORD_bit_len);
			long_divC_word(R, B, &Q_tmp, &R_tmp);
			(*quo)->a[0] = Q_tmp->a[0];
			swap_bignum(&R, &R_tmp);

			bigint_free(&R_tmp);
			bigint_free(&Q_tmp);
		}
		else
		{
			left_shift(*quo, WORD_bit_len);
		}
	}

	for (i = (*quo)->word_len - 1; i >= 0; i--)
	{
		if ((*quo)->a[i] != 0)
			break;
		(*quo)->word_len--;
		(*quo)->a = (WORD *)realloc((*quo)->a, sizeof(WORD)*((*quo)->word_len));
	}
	swap_bignum(rem, &R);

	bigint_free(&R);

	if (A->sign == 1) // 나눠지는 수가 음수인 경우
	{
		bigint_gen(&num1, 0, 1, &one_init);
		Add(&Q3, *quo, num1);
		swap_bignum(&Q3, quo);
		bigint_free(&Q3);
		(*quo)->sign = 1;
		Mul(&M, B, *quo, 'k');
		Sub(&R3, A, M);
		swap_bignum(&R3, rem);
		bigint_free(&R3);
		bigint_free(&M);
	}

}

void long_divC_word(bigint* A, bigint* B, bigint** quo, bigint** rem)
{
	int k = 0;
	int ret = 0;
	bigint* A_tmp = NULL;
	bigint* B_tmp = NULL;

	ret = bit_length(B);

	k = ret - WORD_bit_len * ((B->word_len) - 1);
	k = (WORD_bit_len - k);

	bigint_gen(&A_tmp, A->sign, A->word_len, A->a);
	bigint_gen(&B_tmp, B->sign, B->word_len, B->a);

	left_shift(A_tmp, k);
	left_shift(B_tmp, k);

	long_divCC_word(A_tmp, B_tmp, quo, rem);

	right_shift(*rem, k);

	bigint_free(&A_tmp);
	bigint_free(&B_tmp);
}

void long_divCC_word(bigint* A, bigint* B, bigint** quo, bigint** rem)
{
	WORD Q = 0;
	bigint* BQ = NULL;
	bigint* rem1 = NULL;
	bigint* quo_tmp = NULL;
	int A_word_len = 0;
	int B_word_len = 0;
	WORD W_1 = 0;


	if (WORD_bit_len == 8) // W를 더해주기 위한 값 저장. 
	{
		W_1 = 0xff;
	}
	else if (WORD_bit_len == 32)
	{
		W_1 = 0xffffffff;
	}
	else if (WORD_bit_len == 64)
	{
		W_1 = 0xffffffffffffffff;
	}

	A_word_len = A->word_len;
	B_word_len = B->word_len;

	if (A_word_len == B_word_len)
	{
		Q = A->a[A_word_len - 1] / B->a[B_word_len - 1];
		
	}
	
	if (A_word_len == B_word_len + 1)
	{
		if (A->a[B_word_len] == B->a[B_word_len - 1])
			Q = W_1;
		else
			Q = long_div_2word(A, B);
	}

	bigint_gen(&quo_tmp, 0, 1, &Q);
	Mul(&BQ, B, quo_tmp, 's');
	Sub(&rem1, A, BQ);

	while (rem1->sign == 1)
	{
		Add(rem, rem1, B);
		Q -= 1;
		swap_bignum(rem, &rem1);
		bigint_free(rem);
	}

	bigint_gen(rem, rem1->sign, rem1->word_len, rem1->a);
	bigint_gen(quo, 0, 1, &Q);

	bigint_free(&BQ);
	bigint_free(&rem1);
	bigint_free(&quo_tmp);
}


WORD long_div_2word(bigint* A, bigint* B)
{
	WORD Q = 0;
	WORD R = 0;
	WORD cmp = 0;
	WORD two_w_1 = 0;
	WORD bit = 0;
	WORD tmp = 0;
	int w = 0;
	int j = 0;
	w = WORD_bit_len;
	int word_len = 0;
	int bit_len = 0;

	word_len = B->word_len;
	R = A->a[A->word_len - 1];
	two_w_1 = ((WORD)(1) << (w - 1));
	if(A->word_len != 2)
		bit_len = (A->word_len - 2)* WORD_bit_len;
	
	for (j = w - 1; j >= 0; j--)
	{
		bit = j_bit(A, j + bit_len);
		if (R >= two_w_1)
		{
			Q += ((WORD)(1) << (j));
			tmp = B->a[word_len - 1] - R;
			R += bit;
			R -= tmp;
		}
		else
		{
			cmp = R;
			R = (R << 1) + bit;

			if (R >= B->a[word_len - 1] || R < cmp)
			{
				Q += ((WORD)(1) << (j));
				R -= B->a[word_len - 1];
			}
		}
	}

	return Q;
	

}