#include "bigint_define.h"
#include "bigint_fucnction.h"
#include "bigint_help_function.h"


int sigleword_Squ(WORD* C, WORD A)
{
	WORD A1 = 0; 
	WORD A0 = 0; 

	WORD C1 = 0;
	WORD C0 = 0;
	WORD T1 = 0;

	WORD T = 0;
	int bit_len = 0;

	bit_len = WORD_bit_len / 2;

	A1 = (A >> (bit_len)); // A1 = A >> (w/2)
	A0 = A & (((WORD)(1) << (bit_len)) - 1); // A0 = A mod W^(1/2)


	C1 = A1 * A1; // C1 = A1^2
	C0 = A0 * A0; // C0 = A0^2
	C[1] = C1;
	C[0] = C0;

	T1 = A1 * A0;
	T = T1 * 2;

	if (T < T1) // T1의 최상위 비트가 1인 경우
		C[1] += ((WORD)(1) << (bit_len));

	C[1] += (T >> bit_len); //  상위 워드 비트 길이의 절반 비트 
	C[0] += (T << bit_len); //  하위 워드 비트 길이의 절반 비트

	if (C[0] < C0) // carry 발생
		C[1]++;

	if (C[1] == 0) // 워드 길이가 1인 경우
		return 1;
	return 2;
}

int SquC(bigint** C, bigint* A)
{
	int i = 0;
	int j = 0;
	int ret = 0;
	WORD tmp[2] = { 0, };
	bigint* Bsingleword_squ = NULL;
	bigint* C1 = NULL;
	bigint* C2 = NULL;
	bigint* Btmp = NULL;
	WORD init = 0x00;

	bigint_gen(&C1, 0, 1, &init);
	bigint_gen(&C2, 0, 1, &init);
	for (j = 0; j < A->word_len; j++)
	{
		ret = sigleword_Squ(tmp, A->a[j]); // 단일 워드 곱
		bigint_gen(&Bsingleword_squ, 0, ret, tmp); // 단일 워드 곱의 결과를 bigint 구조체로 생성
		
		left_shift(Bsingleword_squ, 2 * j*WORD_bit_len);
		ret = Add(&Btmp, C1, Bsingleword_squ);
		
		swap_bignum(&C1, &Btmp);
		

		memset(tmp, 0x00, sizeof(WORD) * 2);
		ret = bigint_free(&Bsingleword_squ);
		ret = bigint_free(&Btmp);
	}

	for (j = 0; j < A->word_len; j++)
	{
		for (i = j + 1; i < A->word_len; i++)
		{
			ret = sigleword_Mul(tmp, A->a[i], A->a[j]);
			bigint_gen(&Bsingleword_squ, 0, ret, tmp);

			left_shift(Bsingleword_squ, (i + j)*WORD_bit_len);
			ret = Add(&Btmp, C2, Bsingleword_squ);

			swap_bignum(&C2, &Btmp);

			memset(tmp, 0x00, sizeof(WORD) * 2);
			ret = bigint_free(&Bsingleword_squ);
			ret = bigint_free(&Btmp);
		}
	}

	left_shift(C2, 1); // C2 = 2*C2
	Add(C, C1, C2); // C = C1 + C2

	ret = bigint_free(&C1);
	ret = bigint_free(&C2);
	return SUCCESS;
}


int SquC_K(bigint** C, bigint* A)
{
	bigint* A0 = NULL;
	bigint* A1 = NULL;

	bigint* T1 = NULL;
	bigint* T0 = NULL;

	bigint* R = NULL;
	bigint* S = NULL;

	int n = 0;
	int ret = 0;
	WORD tmp[2] = { 0, };


	n = A->word_len / 2;
	if (n == 0)
	{
		ret = sigleword_Squ(tmp, A->a[0]);
		ret = bigint_gen(C, 0, ret, tmp);
		return SUCCESS;
	}

	ret = bigint_gen(&A1, A->sign, A->word_len, A->a);
	ret = bigint_gen(&A0, A->sign, A->word_len, A->a);

	right_shift(A1, WORD_bit_len * n); // A1 = A >> wn. 절댓값을 안넣어줘도 되는 이유는 단일워드 곱에서 생성하는 bigint구조체의 부호를 무조건 +로 생성하기 때문
	mod(A0, WORD_bit_len * n); // A0 = A mod W^n

	SquC_K(&T1, A1); // T1 = A1^2
	SquC_K(&T0, A0); // T0 = A0^2

	
	left_shift(T1, 2 * n *WORD_bit_len); // T1 = T1 * W^(2n)

	Add(&R, T1, T0); // R = T1 + T0
	MulC(&S, A1, A0); // S = A1 * A0
	left_shift(S, 1 + (WORD_bit_len * n)); // S << wn + 1
	Add(C, R, S); // C = R + S



	ret = bigint_free(&A0);
	ret = bigint_free(&A1);
	ret = bigint_free(&T0);
	ret = bigint_free(&T1);
	
	ret = bigint_free(&R);
	ret = bigint_free(&S);


	return SUCCESS;
}


int Squ(bigint** C, bigint* A, char k)
{
	bigint* A_tmp = NULL;
	int zero_val_A = 0;
	int one_val_A = 0;

	int ret = 0;
	int A_sign = 0;

	WORD zero_init = 0x00;

	zero_val_A = is_zero(*A);

	one_val_A = is_one(*A);


	if (zero_val_A == zero) // A == 0 -> A^2 == 0
	{
		ret = bigint_gen(C, 0, 1, &zero_init);
		if (ret == FAIL)
		{
			printf("memory allocation fail (Squ)!!!\n");
			return FAIL;
		}
		return SUCCESS;
	}

	if (one_val_A == one) // A == +- 1 -> A == 1
	{
		ret = bigint_gen(C, 0, A->word_len, A->a);
		if (ret == FAIL)
		{
			printf("memory allocation fail (Squ)!!!\n");
			return FAIL;
		}
		return SUCCESS;
	}

	bigint_gen(&A_tmp, A->sign, A->word_len, A->a);

	if (k == 'k') // 카라츄바
	{

		K_exp(A_tmp, A_tmp);
		SquC_K(C, A_tmp);

		return SUCCESS;
	}

	SquC(C, A_tmp); // school book

	bigint_free(&A_tmp);

	return SUCCESS;
}