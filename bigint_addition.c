#include "bigint_define.h"
#include "bigint_fucnction.h"
#include "bigint_help_function.h"



/* AddC
Input : A, B (A_sign = B_sign)
Output : C  such that A + B, C_word_len <= max{A_word_len, B_word_len} + 1

memory allocation C in AddC function 
*/
int AddC(bigint** C, bigint* A, bigint* B) // C = A + B
{

	unsigned int c = 0; // 넘어온 carry 저장 변수
	unsigned int c1 = 0; // 넘어갈 carry 저장 변수
	int i = 0;
	int ret = 0;
	int word_len = 0;
	int size = 0;
	WORD* Add = NULL;

	if (B->word_len > A->word_len) //  A(word_len = n) + B(word_len = m) = C(word_len <= max(n, m) + 1) 	
	{
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

	size = (word_len + 1) * sizeof(WORD);
	Add = (WORD *)malloc(size);
	memset(Add, 0x00, size);

	for (i = 0; i < word_len; i++)
	{
		Add[i] = (A->a[i] + B->a[i]); // 두 단일 워드의 덧셈. C = A + B mod 2^w , w : word bit length

		if (Add[i] < A->a[i]) // 넘어갈 carry 발생. 
		{
			c1 = 1;
		}

		Add[i] = Add[i] + c; // 넘어온 carry를 더해준다.

		if (Add[i] < c) // 넘어온 carry와 덧셈을 했을 경우 넘어갈 carry가 발생한 경우. C + c = 0 , c = 1인 경우 
		{
			c1 = c1 + 1;
		}

		c = c1; // 다음 워드의 덧셈을 위해 현재 워드의 덧셈에서 넘어갈 carry는 다음 워드의 넘어온 carry가 된다. 두 정수 A,B의 결과값의 WORD_len는 최대 max(A_word_len, B_word_len) + 1이다.
		c1 = 0; // 다음 워드의 덧셈을 위해 넘어갈 carry를 0으로 변경한다.
	}


	if (c == 1)
	{
		Add[i] = c; // 두 정수의 덧셈이 끝난 후 최상위 WORD로 넘어갈 carry가 있으면 최상위 WORD에 1 저장
		word_len++; // 최상위 WORD로 넘어갈 carry가 있는 경우 word_len 1 증가
					// 최상위 WORD로 넘어갈 carry가 없으면 두 정수의 덧셈의 결과값의 word_len은 max(A_word_len, B_word_len)이 된다.
	}



	ret = bigint_gen(C, A->sign, word_len, Add); // 동일한 부호의 덧셈이므로 결과값도 동일한 부호이다.
	if (ret == FAIL)
	{
		printf("ADDC 메모리 할당 실패\n");
		return FAIL;
	}

	memset(Add, 0x00, size);
	free(Add);
	Add = NULL;

	return SUCCESS;
}




/* Add
Input : A, B 
Output : C  such that A + B, C_word_len <= max{A_word_len, B_word_len} + 1
*/
int Add(bigint** C, bigint* A, bigint* B) // 두 정수의 덧셈
{
	
	int val = 0; // A가 0인지 판단해주는 함수의 결과값을 저장하기 위한 변수
	int val1 = 0; // B가 0인지 판단해주는 함수의 결과값을 저장하기 위한 변수
	int ret = 0;
	bigint* tmp = NULL;

	val = is_zero(*A);
	val1 = is_zero(*B);

	if (val == zero) // A = 0 -> A + B = B
	{
		ret = bigint_gen(C, B->sign, B->word_len, B->a);
		if (ret == FAIL)
		{
			printf("ADD 메모리 할당 실패\n");
			return FAIL;
		}
		return SUCCESS;
	}
	else if (val1 == zero) // B = 0 -> A + B = A
	{
		ret = bigint_gen(C, A->sign, A->word_len, A->a);
		if (ret == FAIL)
		{
			printf("ADD 메모리 할당 실패\n");
			return FAIL;
		}
		return SUCCESS;
	}
	else if (A->sign == 0 && B->sign == 1) // A > 0, B < 0 -> A + B = A - |B|
	{
		ret = bigint_gen(&tmp, 0, B->word_len, B->a);  // B의 부호를 바꾸지 않기 위해 C에 B의 값을 저장
		if (ret == FAIL)
		{
			printf("ADD 메모리 할당 실패\n");
			return FAIL;
		}

		ret = Sub(C, A, tmp); // A - tmp = A - |B| = A + B = C
		ret = bigint_free(&tmp);
		return SUCCESS;
	}
	else if (A->sign == 1 && B->sign == 0) // A < 0, B > 0 -> A + B = B - |A|
	{
		ret = bigint_gen(&tmp, 0, A->word_len, A->a);  // A의 부호를 바꾸지 않기 위해 C에 B의 값을 저장
		if (ret == FAIL)
		{
			printf("ADD 메모리 할당 실패\n");
			return FAIL;
		}

		ret = Sub(C, B, tmp); // C = B - tmp = B - |A| = B + A
		ret = bigint_free(&tmp);

		return SUCCESS;
	}

	ret = AddC(C, A, B);
	return SUCCESS;
}