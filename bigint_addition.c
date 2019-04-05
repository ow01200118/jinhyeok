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

	unsigned int c = 0; // �Ѿ�� carry ���� ����
	unsigned int c1 = 0; // �Ѿ carry ���� ����
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
		Add[i] = (A->a[i] + B->a[i]); // �� ���� ������ ����. C = A + B mod 2^w , w : word bit length

		if (Add[i] < A->a[i]) // �Ѿ carry �߻�. 
		{
			c1 = 1;
		}

		Add[i] = Add[i] + c; // �Ѿ�� carry�� �����ش�.

		if (Add[i] < c) // �Ѿ�� carry�� ������ ���� ��� �Ѿ carry�� �߻��� ���. C + c = 0 , c = 1�� ��� 
		{
			c1 = c1 + 1;
		}

		c = c1; // ���� ������ ������ ���� ���� ������ �������� �Ѿ carry�� ���� ������ �Ѿ�� carry�� �ȴ�. �� ���� A,B�� ������� WORD_len�� �ִ� max(A_word_len, B_word_len) + 1�̴�.
		c1 = 0; // ���� ������ ������ ���� �Ѿ carry�� 0���� �����Ѵ�.
	}


	if (c == 1)
	{
		Add[i] = c; // �� ������ ������ ���� �� �ֻ��� WORD�� �Ѿ carry�� ������ �ֻ��� WORD�� 1 ����
		word_len++; // �ֻ��� WORD�� �Ѿ carry�� �ִ� ��� word_len 1 ����
					// �ֻ��� WORD�� �Ѿ carry�� ������ �� ������ ������ ������� word_len�� max(A_word_len, B_word_len)�� �ȴ�.
	}



	ret = bigint_gen(C, A->sign, word_len, Add); // ������ ��ȣ�� �����̹Ƿ� ������� ������ ��ȣ�̴�.
	if (ret == FAIL)
	{
		printf("ADDC �޸� �Ҵ� ����\n");
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
int Add(bigint** C, bigint* A, bigint* B) // �� ������ ����
{
	
	int val = 0; // A�� 0���� �Ǵ����ִ� �Լ��� ������� �����ϱ� ���� ����
	int val1 = 0; // B�� 0���� �Ǵ����ִ� �Լ��� ������� �����ϱ� ���� ����
	int ret = 0;
	bigint* tmp = NULL;

	val = is_zero(*A);
	val1 = is_zero(*B);

	if (val == zero) // A = 0 -> A + B = B
	{
		ret = bigint_gen(C, B->sign, B->word_len, B->a);
		if (ret == FAIL)
		{
			printf("ADD �޸� �Ҵ� ����\n");
			return FAIL;
		}
		return SUCCESS;
	}
	else if (val1 == zero) // B = 0 -> A + B = A
	{
		ret = bigint_gen(C, A->sign, A->word_len, A->a);
		if (ret == FAIL)
		{
			printf("ADD �޸� �Ҵ� ����\n");
			return FAIL;
		}
		return SUCCESS;
	}
	else if (A->sign == 0 && B->sign == 1) // A > 0, B < 0 -> A + B = A - |B|
	{
		ret = bigint_gen(&tmp, 0, B->word_len, B->a);  // B�� ��ȣ�� �ٲ��� �ʱ� ���� C�� B�� ���� ����
		if (ret == FAIL)
		{
			printf("ADD �޸� �Ҵ� ����\n");
			return FAIL;
		}

		ret = Sub(C, A, tmp); // A - tmp = A - |B| = A + B = C
		ret = bigint_free(&tmp);
		return SUCCESS;
	}
	else if (A->sign == 1 && B->sign == 0) // A < 0, B > 0 -> A + B = B - |A|
	{
		ret = bigint_gen(&tmp, 0, A->word_len, A->a);  // A�� ��ȣ�� �ٲ��� �ʱ� ���� C�� B�� ���� ����
		if (ret == FAIL)
		{
			printf("ADD �޸� �Ҵ� ����\n");
			return FAIL;
		}

		ret = Sub(C, B, tmp); // C = B - tmp = B - |A| = B + A
		ret = bigint_free(&tmp);

		return SUCCESS;
	}

	ret = AddC(C, A, B);
	return SUCCESS;
}