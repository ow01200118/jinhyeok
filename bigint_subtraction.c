#include "bigint_define.h"
#include "bigint_fucnction.h"
#include "bigint_help_function.h"


int SubC(bigint** C, bigint* A, bigint* B) // 음이 아닌 부호의 두 정수 A,B(A >= B)의 뺄셈 
{

	unsigned int b = 0; // 빌려오거나 빌려준 수를 저장
	int i = 0;
	int size = 0;
	WORD* Sub = NULL;
	int ret = 0;

	int word_len = 0;
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

	if (B->word_len > A->word_len) // 두 정수 A, B의 뺄셈의 경우 결과값의 최대 WORD_len은 max(A_word_len, B_word_len)이다.
	{							   // 두 음이 아닌 정수 A(word_len = n) , B(word_len = m)의 뺄셈은 max(n, m)워드 정수로 간주하여 계산한다.
		word_len = B->word_len;
		A->a = (WORD *)realloc(A->a, word_len * sizeof(WORD));
		memset(&(A->a[A->word_len]), 0x00, (B->word_len - A->word_len) * sizeof(WORD));
	}
	else if (B->word_len < A->word_len)
	{
		word_len = A->word_len;
		B->a = (WORD *)realloc(B->a, word_len * sizeof(WORD));
		memset(&(B->a[B->word_len]), 0x00, (A->word_len - B->word_len) * sizeof(WORD));
	}
	else
		word_len = A->word_len;

	size = word_len * sizeof(WORD);
	Sub = (WORD *)malloc(size);
	memset(Sub, 0x00, size);

	for (i = 0; i < word_len; i++)
	{

		if (A->a[i] < b) // A->a[i] = 0, b = 1인 경우
		{
			Sub[i] = (W_1 + A->a[i]); // W_1 = W - 1. 앞 워드에서 숫자를 빌려온 경우
			b = 1; // 빌려온 수 = 1
		}
		else // A->a[i]가 0이 아닌 경우
		{
			Sub[i] = A->a[i] - b;  // 빌려준 수를 빼준다.
			b = 0;
		}


		if (Sub[i] < B->a[i])  // 앞 워드에서 빌려와야 하는 경우
		{
			Sub[i] = (W_1 - B->a[i]) + Sub[i] + 1; // 앞 워드에서 빌려온후 뺄셈을 해준다.
			b = b + 1; // 빌려온 수를 더해준다.
		}
		else
		{
			Sub[i] = Sub[i] - B->a[i]; // 빌릴 필요가 없으니 그냥 뺄셈을 해준다.
		}
	}


	for (i = word_len - 1; i >= 0; i--) // 뺄셈 결과값의 word_len 측정
	{
		if (Sub[i] == 0)
		{
			word_len--;
		}
		else
		{
			ret = bigint_gen(C, 0, word_len, Sub);
			if (ret == FAIL)
			{
				printf("SUBC 메모리 할당 실패\n");
				return FAIL;
			}

			memset(Sub, 0x00, size);
			free(Sub);
			Sub = NULL;

			return SUCCESS;
		}
	}

	ret = bigint_gen(C, 0, 1, Sub);
	if (ret == FAIL)
	{
		printf("SUBC 메모리 할당 실패\n");
		return FAIL;
	}

	memset(Sub, 0x00, size);
	free(Sub);
	Sub = NULL;


	return SUCCESS;
}

int Sub(bigint** C, bigint* A, bigint* B)
{

	int A_is_zero_val = 0;
	int B_is_zero_val = 0;
	int cmp_val = 0;
	int A_sign = 0;
	int B_sign = 0;
	int ret = 0;
	bigint* tmp = NULL;


	A_sign = A->sign;
	B_sign = B->sign;

	A_is_zero_val = is_zero(*A);
	B_is_zero_val = is_zero(*B);
	cmp_val = compare(A, B);

	if (A_is_zero_val == zero) // A == 0, A - B = -B
	{
		ret = bigint_gen(C, B->sign ^ 1, B->word_len, B->a); // B의 부호 반전
		if (ret == FAIL)
		{
			printf("ADD 메모리 할당 실패\n");
			return FAIL;
		}
		return SUCCESS;
	}
	else if (B_is_zero_val == zero) // B == 0, A - B = A
	{
		ret = bigint_gen(C, A->sign, A->word_len, A->a); // B의 부호 반전
		if (ret == FAIL)
		{
			printf("ADD 메모리 할당 실패\n");
			return FAIL;
		}
		return SUCCESS;
	}
	else if (A_sign == 0 && B_sign == 0 && (cmp_val == -1 || cmp_val == 0)) // A > 0, B > 0, A >= B인 경우 A - B = C
	{
		ret = SubC(C, A, B);

		return SUCCESS;
	}
	else if (A_sign == 0 && B_sign == 0 && cmp_val == 1) // A > 0 , B > 0, A < B 인 경우 A - B = - (B - A)
	{
		ret = SubC(C, B, A);
		(*C)->sign = 1;


		return SUCCESS;
	}
	else if (A_sign == 1 && B_sign == 1 && (cmp_val == -1 || cmp_val == 0)) // A < 0, B < 0, A >= B 인 경우 A - B = |B| - |A|
	{
		ret = SubC(C, B, A);

		return SUCCESS;
	}
	else if (A_sign == 1 && B_sign == 1 && cmp_val == 1) // A < 0, B < 0, A < B 인 경우 A - B = - (|A| - |B|)
	{
		ret = SubC(C, A, B);
		(*C)->sign = 1;

		return SUCCESS;
	}
	else if (A_sign == 0 && B_sign == 1) // A > 0 , B < 0 인 경우 A - B = A + |B|
	{
		ret = bigint_gen(&tmp, 0, B->word_len, B->a);
		if (ret == FAIL)
		{
			printf("SUB 메모리 할당 실패\n");
			return FAIL;
		}

		ret = Add(C, A, tmp);
		ret = bigint_free(&tmp);
		return SUCCESS;
	}
	else if (A_sign == 1 && B_sign == 0) // A < 0, B > 0 인 경우 A - B = -(|A| + B)
	{
		ret = bigint_gen(&tmp, 0, A->word_len, A->a);
		if (ret == FAIL)
		{
			printf("SUB 메모리 할당 실패\n");
			return FAIL;
		}
		ret = Add(C, tmp, B);
		ret = bigint_free(&tmp);
		(*C)->sign = 1;

		return SUCCESS;
	}

	return FAIL;
}
















int SubC_k(bigint** C, bigint* A, bigint* B) // 음이 아닌 부호의 두 정수 A,B(A >= B)의 뺄셈 
{

	unsigned int b = 0; // 빌려오거나 빌려준 수를 저장
	int i = 0;
	int size = 0;
	WORD* Sub = NULL;
	int ret = 0;

	int word_len = 0;
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

	if (B->word_len > A->word_len) // 두 정수 A, B의 뺄셈의 경우 결과값의 최대 WORD_len은 max(A_word_len, B_word_len)이다.
	{							   // 두 음이 아닌 정수 A(word_len = n) , B(word_len = m)의 뺄셈은 max(n, m)워드 정수로 간주하여 계산한다.
		word_len = B->word_len;
		A->a = (WORD *)realloc(A->a, word_len * sizeof(WORD));
		memset(&(A->a[A->word_len]), 0x00, (B->word_len - A->word_len) * sizeof(WORD));
	}
	else if (B->word_len < A->word_len)
	{
		word_len = A->word_len;
		B->a = (WORD *)realloc(B->a, word_len * sizeof(WORD));
		memset(&(B->a[B->word_len]), 0x00, (A->word_len - B->word_len) * sizeof(WORD));
	}
	else
		word_len = A->word_len;

	size = word_len * sizeof(WORD);
	Sub = (WORD *)malloc(size);
	memset(Sub, 0x00, size);

	for (i = 0; i < word_len; i++)
	{

		if (A->a[i] < b) // A->a[i] = 0, b = 1인 경우
		{
			Sub[i] = (W_1 + A->a[i]); // W_1 = W - 1. 앞 워드에서 숫자를 빌려온 경우
			b = 1; // 빌려온 수 = 1
		}
		else // A->a[i]가 0이 아닌 경우
		{
			Sub[i] = A->a[i] - b;  // 빌려준 수를 빼준다.
			b = 0;
		}


		if (Sub[i] < B->a[i])  // 앞 워드에서 빌려와야 하는 경우
		{
			Sub[i] = (W_1 - B->a[i]) + Sub[i] + 1; // 앞 워드에서 빌려온후 뺄셈을 해준다.
			b = b + 1; // 빌려온 수를 더해준다.
		}
		else
		{
			Sub[i] = Sub[i] - B->a[i]; // 빌릴 필요가 없으니 그냥 뺄셈을 해준다.
		}
	}


	
		ret = bigint_gen(C, 0, word_len, Sub);
		if(ret == FAIL)
		{
			printf("SUBC 메모리 할당 실패\n");
			return FAIL;
		}

		memset(Sub, 0x00, size);
		free(Sub);
		Sub = NULL;

		return SUCCESS;
		
	
}

int Sub_k(bigint** C, bigint* A, bigint* B)
{

	int A_is_zero_val = 0;
	int B_is_zero_val = 0;
	int cmp_val = 0;
	int A_sign = 0;
	int B_sign = 0;
	int ret = 0;
	bigint* tmp = NULL;


	A_sign = A->sign;
	B_sign = B->sign;

	A_is_zero_val = is_zero(*A);
	B_is_zero_val = is_zero(*B);
	cmp_val = compare(A, B);

	if (A_is_zero_val == zero) // A == 0, A - B = -B
	{
		ret = bigint_gen(C, B->sign ^ 1, B->word_len, B->a); // B의 부호 반전
		if (ret == FAIL)
		{
			printf("ADD 메모리 할당 실패\n");
			return FAIL;
		}
		return SUCCESS;
	}
	else if (B_is_zero_val == zero) // B == 0, A - B = A
	{
		ret = bigint_gen(C, A->sign, A->word_len, A->a); // B의 부호 반전
		if (ret == FAIL)
		{
			printf("ADD 메모리 할당 실패\n");
			return FAIL;
		}
		return SUCCESS;
	}
	else if (A_sign == 0 && B_sign == 0 && (cmp_val == -1 || cmp_val == 0)) // A > 0, B > 0, A >= B인 경우 A - B = C
	{
		ret = SubC_k(C, A, B);

		return SUCCESS;
	}
	else if (A_sign == 0 && B_sign == 0 && cmp_val == 1) // A > 0 , B > 0, A < B 인 경우 A - B = - (B - A)
	{
		ret = SubC_k(C, B, A);
		(*C)->sign = 1;


		return SUCCESS;
	}
	else if (A_sign == 1 && B_sign == 1 && (cmp_val == -1 || cmp_val == 0)) // A < 0, B < 0, A >= B 인 경우 A - B = |B| - |A|
	{
		ret = SubC_k(C, B, A);

		return SUCCESS;
	}
	else if (A_sign == 1 && B_sign == 1 && cmp_val == 1) // A < 0, B < 0, A < B 인 경우 A - B = - (|A| - |B|)
	{
		ret = SubC_k(C, A, B);
		(*C)->sign = 1;

		return SUCCESS;
	}
	else if (A_sign == 0 && B_sign == 1) // A > 0 , B < 0 인 경우 A - B = A + |B|
	{
		ret = bigint_gen(&tmp, 0, B->word_len, B->a);
		if (ret == FAIL)
		{
			printf("SUB 메모리 할당 실패\n");
			return FAIL;
		}

		ret = Add(C, A, tmp);
		ret = bigint_free(&tmp);
		return SUCCESS;
	}
	else if (A_sign == 1 && B_sign == 0) // A < 0, B > 0 인 경우 A - B = -(|A| + B)
	{
		ret = bigint_gen(&tmp, 0, A->word_len, A->a);
		if (ret == FAIL)
		{
			printf("SUB 메모리 할당 실패\n");
			return FAIL;
		}
		ret = Add(C, tmp, B);
		ret = bigint_free(&tmp);
		(*C)->sign = 1;

		return SUCCESS;
	}

	return FAIL;
}