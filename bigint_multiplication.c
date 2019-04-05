#include "bigint_define.h"
#include "bigint_fucnction.h"
#include "bigint_help_function.h"



int sigleword_Mul(WORD* C, WORD A, WORD B) // C는 2워드 or 1워드, 워드 길이 return
{

	WORD A1 = 0; // A = A1 * W^(1/2) + A0
	WORD A0 = 0;
	WORD B1 = 0; // B = B1 * W^(1/2) + B0
	WORD B0 = 0;
	WORD C1 = 0; // A1*B1
	WORD C0 = 0; // A0*B0
	WORD T1 = 0; // A1*B0
	WORD T0 = 0; // A0*B1
	WORD T = 0; // T1 + T0
	int half_word_bit_len = 0;

	half_word_bit_len = WORD_bit_len / 2;

	A1 = (A >> (half_word_bit_len));
	A0 = A & (((WORD)(1) << (half_word_bit_len)) - 1);
	B1 = (B >> (WORD_bit_len / 2));
	B0 = B & (((WORD)(1) << (half_word_bit_len)) - 1);

	C1 = A1 * B1;
	C0 = A0 * B0;
	C[1] = C1;
	C[0] = C0;

	T1 = A1 * B0;
	T0 = A0 * B1;

	T = T1 + T0;

	if (T < T1) // carry 발생
		C[1] += ((WORD)(1) << (half_word_bit_len));

	C[1] += (T >> half_word_bit_len); // T * W^(1/2)를 하면 단일 워드의 상위비트는 carry 발생
	C[0] += (T << half_word_bit_len);

	if (C[0] < C0) // carry 발생
		C[1]++;

	//if (C[1] == 0) // 워드 길이 = 1
	//	return 1;

	return 2; // 워드 길이 = 2 
}

int MulC(bigint** C, bigint* A, bigint* B)
{
	
	bigint* Bsigleword_mul = NULL;
	bigint* Btmp = NULL;
	int i = 0;
	int j = 0;
	int ret = 0;
	WORD tmp[2] = { 0, };
	WORD zero_init = 0;


	ret = bigint_gen(C, 0, 1, &zero_init);
	if (ret == FAIL)
	{
		printf("memory allocation fail(MulC)!!!\n");
		return FAIL;
	}

	for (j = 0; j < A->word_len; j++)
	{
		for (i = 0; i < B->word_len; i++)
		{
			ret = sigleword_Mul(tmp, A->a[j], B->a[i]);
			ret = bigint_gen(&Bsigleword_mul, 0, ret, tmp); // T = A[j] * B[i]

			left_shift(Bsigleword_mul, WORD_bit_len*(i + j)); // T = T*W(i+j)
			ret = Add(&Btmp, *C, Bsigleword_mul); // C = C + T
			
			swap_bignum(C, &Btmp); // Btmp <-> C

			ret = bigint_free(&Btmp);
			ret = bigint_free(&Bsigleword_mul);
			memset(tmp, 0x00, sizeof(WORD) * 2);
		}
	}
	

	return SUCCESS;
}

int K_exp(bigint* A, bigint* B) // A_word_len >= B_word_len
{

	int quo = 0;
	int num = 0;
	int exp = 0;
	int ret = 0;


	quo = A->word_len;
	while (quo != 1)
	{
		num++;
		quo = (quo / 2);
	}
	exp = ((int)(1) << num);

	ret = compare(A, B);
	if (ret == A_equal_B) // 제곱 카라츄바일 경우
	{
		if (exp == A->word_len) // 2의 지수승의 배수일 경우
			return SUCCESS;

		exp = (exp << 1); //2의 지수승의 배수가 아닌 경우 
		A->a = (WORD *)realloc(A->a, exp * sizeof(WORD)); // 2의 지수승의 길이만큼 확장
		memset(&(A->a[A->word_len]), 0x00, (exp - A->word_len) * sizeof(WORD)); // 확장된 워드는 0으로 초기화
		A->word_len = exp; // word_len 갱신

		B->a = (WORD *)realloc(B->a, exp * sizeof(WORD)); // 2의 지수승의 길이만큼 확장
		memset(&(B->a[B->word_len]), 0x00, (exp - B->word_len) * sizeof(WORD)); // 확장된 워드는 0으로 초기화
		B->word_len = exp; // word_len 갱신

		return SUCCESS;
	}


	if (A->word_len == B->word_len) 
	{
		if (exp == A->word_len) // 이미 word_len가 2의 지수승이기 때문에 확장할 필요가 없음
			return SUCCESS;
	}

	if (exp == A->word_len) // A->word_len != B->word_len 인데 A->word_len이 2의 지수승일 경우
	{
		B->a = (WORD *)realloc(B->a, exp * sizeof(WORD));
		memset(&(B->a[B->word_len]), 0x00, (exp - B->word_len) * sizeof(WORD));
		B->word_len = exp;
	}
	else // A->word_len가 2의 지수승이 아닐 경우
	{
		exp = exp * 2;
		A->a = (WORD *)realloc(A->a, exp * sizeof(WORD));
		memset(&(A->a[A->word_len]), 0x00, (exp - A->word_len) * sizeof(WORD));
		A->word_len = exp;

		B->a = (WORD *)realloc(B->a, exp * sizeof(WORD));
		memset(&(B->a[B->word_len]), 0x00, (exp - B->word_len) * sizeof(WORD));
		B->word_len = exp;
	}

	return SUCCESS;
}

int MulC_K(bigint** C, bigint* A, bigint* B) // A, B 의 word_len은 2의 지수승. 
{

	bigint* A0 = NULL;
	bigint* A1 = NULL;
	bigint* B0 = NULL;
	bigint* B1 = NULL;
	bigint* T1 = NULL;
	bigint* T0 = NULL;
	bigint* T3 = NULL;
	bigint* R = NULL;
	bigint* S = NULL;
	bigint* S1 = NULL;
	bigint* S0 = NULL;
	bigint* S3 = NULL;
	bigint* S4 = NULL;

	int sign = 0;
	int n = 0;
	int ret = 0;
	WORD tmp[2] = { 0, };
	WORD *R_arr = NULL;


	n = (A->word_len) / 2;
	if (n == 0)
	{
		ret = sigleword_Mul(tmp, A->a[0], B->a[0]);
		ret = bigint_gen(C, 0, ret, tmp);
		memset(tmp, 0x00, sizeof(WORD) * 2);
		return SUCCESS;
	}

	


	/*ret = bigint_gen(&A1, A->sign, A->word_len, A->a); 
	ret = bigint_gen(&A0, A->sign, A->word_len, A->a);
	ret = bigint_gen(&B1, B->sign, B->word_len, B->a);
	ret = bigint_gen(&B0, B->sign, B->word_len, B->a);

	right_shift(A1, WORD_bit_len * n); //A1 = A >> wn
	right_shift(B1, WORD_bit_len * n); //B1 = B >> wn

	
	mod(A0, WORD_bit_len * n); // A0 = A mod W^n
	mod(B0, WORD_bit_len * n); // B0 = B mod W^n*/
	

	ret = bigint_gen(&A1, A->sign, n, &A->a[n]);
	ret = bigint_gen(&A0, A->sign, n, A->a);
	ret = bigint_gen(&B1, B->sign, n, &B->a[n]);
	ret = bigint_gen(&B0, B->sign, n, B->a);

	MulC_K(&T1, A1, B1); // T1 = A1 * B1
	
	MulC_K(&T0, A0, B0); // T0 = A0 * B0
	
	/*R_arr = (WORD *)malloc(4 * n * sizeof(WORD));
	memcpy(R_arr, T0->a, 2 * n * sizeof(WORD));
	memcpy(&R_arr[2*n], T1->a, 2 * n * sizeof(WORD));

	bigint_gen(&R, 0, 4 * n, R_arr);*/
	
	ret = bigint_gen(&T3, T1->sign, T1->word_len, T1->a); // T3 = T1
	left_shift(T3, 2 * n *WORD_bit_len); // T3 = T1 * W^(2n)

	Add(&R, T3, T0); // R = T3 + T0

	Sub_k(&S1, A0, A1); // S1 = A0 - A1
	Sub_k(&S0, B1, B0); // S0 = B1 - B0

	sign = ((S1->sign) ^ (S0->sign));
	S1->sign = 0; // |S1|
	S0->sign = 0; // |S0|

	/*if (S1->word_len >= S0->word_len)
		K_exp(S1, S0);
	else
		K_exp(S0, S1);*/


	MulC_K(&S, S1, S0); // S = |S1| * |S0|
	
	S->sign = sign; 

	Add(&S3, S, T1); // S3 = S + T1
	Add(&S4, S3, T0); // S4 = S3 + T0

	left_shift(S4, n*WORD_bit_len); // S4 = S4 * W^n
	Add(C, R, S4);

	ret = bigint_free(&A0);
	ret = bigint_free(&A1);
	ret = bigint_free(&B0);
	ret = bigint_free(&B1);
	ret = bigint_free(&T0);
	ret = bigint_free(&T1);
	ret = bigint_free(&T3);
	ret = bigint_free(&R);
	ret = bigint_free(&S);
	ret = bigint_free(&S0);
	ret = bigint_free(&S1);
	ret = bigint_free(&S3);
	ret = bigint_free(&S4);

	return SUCCESS;
}


int Mul(bigint** C, bigint* A, bigint* B, char k)
{

	int zero_val_A = 0;
	int one_val_A = 0;
	int zero_val_B = 0;
	int one_val_B = 0;
	int ret = 0;
	int A_sign = 0;
	int B_sign = 0;
	WORD zero_init = 0x00;
	bigint* A_tmp = NULL;
	bigint* B_tmp = NULL;

	zero_val_A = is_zero(*A);
	zero_val_B = is_zero(*B);
	one_val_A = is_one(*A);
	one_val_B = is_one(*B);

	if (zero_val_A == zero || zero_val_B == zero) // A == 0 or B == 0 -> C == 0
	{
		ret = bigint_gen(C, 0, 1, &zero_init);
		if (ret == FAIL)
		{
			printf("memory allocation fail (Mul)!!!\n");
			return FAIL;
		}
		return SUCCESS;
	}

	if (one_val_A == one) // A == +- 1 -> C == +- B
	{
		ret = bigint_gen(C, A->sign ^ B->sign, B->word_len, B->a);
		if (ret == FAIL)
		{
			printf("memory allocation fail (Mul)!!!\n");
			return FAIL;
		}
		return SUCCESS;
	}

	if (one_val_B == one) // B == +- 1 -> C == +- A
	{
		ret = bigint_gen(C, A->sign ^ B->sign, A->word_len, A->a);
		if (ret == FAIL)
		{
			printf("memory allocation fail (Mul)!!!\n");
			return FAIL;
		}
		return SUCCESS;
	}

	bigint_gen(&A_tmp, A->sign, A->word_len, A->a);
	bigint_gen(&B_tmp, B->sign, B->word_len, B->a);

	A_sign = A->sign;
	B_sign = B->sign;
	A_tmp->sign = 0;
	B_tmp->sign = 0;

	if (k == 'k') // 카라추바
	{
		if (A->word_len >= B->word_len)
			K_exp(A_tmp, B_tmp);
		else
			K_exp(B_tmp, A_tmp);
		MulC_K(C, A_tmp, B_tmp);
		(*C)->sign = A_sign ^ B_sign;

		if ((*C)->a[(*C)->word_len - 1] == 0x00)
		{
			(*C)->word_len--;
			(*C)->a = (WORD *)realloc((*C)->a, (*C)->word_len * sizeof(WORD));
		}
		bigint_free(&A_tmp);
		bigint_free(&B_tmp);
		return SUCCESS;
	}

	MulC(C, A_tmp, B_tmp);
	(*C)->sign = A_sign ^ B_sign;

	if ((*C)->a[(*C)->word_len - 1] == 0x00)
	{
		(*C)->word_len--;
		(*C)->a = (WORD *)realloc((*C)->a, (*C)->word_len * sizeof(WORD));
	}

	bigint_free(&A_tmp);
	bigint_free(&B_tmp);
	return SUCCESS;

}