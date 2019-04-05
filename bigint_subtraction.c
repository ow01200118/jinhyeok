#include "bigint_define.h"
#include "bigint_fucnction.h"
#include "bigint_help_function.h"


int SubC(bigint** C, bigint* A, bigint* B) // ���� �ƴ� ��ȣ�� �� ���� A,B(A >= B)�� ���� 
{

	unsigned int b = 0; // �������ų� ������ ���� ����
	int i = 0;
	int size = 0;
	WORD* Sub = NULL;
	int ret = 0;

	int word_len = 0;
	WORD W_1 = 0;


	if (WORD_bit_len == 8) // W�� �����ֱ� ���� �� ����. 
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

	if (B->word_len > A->word_len) // �� ���� A, B�� ������ ��� ������� �ִ� WORD_len�� max(A_word_len, B_word_len)�̴�.
	{							   // �� ���� �ƴ� ���� A(word_len = n) , B(word_len = m)�� ������ max(n, m)���� ������ �����Ͽ� ����Ѵ�.
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

		if (A->a[i] < b) // A->a[i] = 0, b = 1�� ���
		{
			Sub[i] = (W_1 + A->a[i]); // W_1 = W - 1. �� ���忡�� ���ڸ� ������ ���
			b = 1; // ������ �� = 1
		}
		else // A->a[i]�� 0�� �ƴ� ���
		{
			Sub[i] = A->a[i] - b;  // ������ ���� ���ش�.
			b = 0;
		}


		if (Sub[i] < B->a[i])  // �� ���忡�� �����;� �ϴ� ���
		{
			Sub[i] = (W_1 - B->a[i]) + Sub[i] + 1; // �� ���忡�� �������� ������ ���ش�.
			b = b + 1; // ������ ���� �����ش�.
		}
		else
		{
			Sub[i] = Sub[i] - B->a[i]; // ���� �ʿ䰡 ������ �׳� ������ ���ش�.
		}
	}


	for (i = word_len - 1; i >= 0; i--) // ���� ������� word_len ����
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
				printf("SUBC �޸� �Ҵ� ����\n");
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
		printf("SUBC �޸� �Ҵ� ����\n");
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
		ret = bigint_gen(C, B->sign ^ 1, B->word_len, B->a); // B�� ��ȣ ����
		if (ret == FAIL)
		{
			printf("ADD �޸� �Ҵ� ����\n");
			return FAIL;
		}
		return SUCCESS;
	}
	else if (B_is_zero_val == zero) // B == 0, A - B = A
	{
		ret = bigint_gen(C, A->sign, A->word_len, A->a); // B�� ��ȣ ����
		if (ret == FAIL)
		{
			printf("ADD �޸� �Ҵ� ����\n");
			return FAIL;
		}
		return SUCCESS;
	}
	else if (A_sign == 0 && B_sign == 0 && (cmp_val == -1 || cmp_val == 0)) // A > 0, B > 0, A >= B�� ��� A - B = C
	{
		ret = SubC(C, A, B);

		return SUCCESS;
	}
	else if (A_sign == 0 && B_sign == 0 && cmp_val == 1) // A > 0 , B > 0, A < B �� ��� A - B = - (B - A)
	{
		ret = SubC(C, B, A);
		(*C)->sign = 1;


		return SUCCESS;
	}
	else if (A_sign == 1 && B_sign == 1 && (cmp_val == -1 || cmp_val == 0)) // A < 0, B < 0, A >= B �� ��� A - B = |B| - |A|
	{
		ret = SubC(C, B, A);

		return SUCCESS;
	}
	else if (A_sign == 1 && B_sign == 1 && cmp_val == 1) // A < 0, B < 0, A < B �� ��� A - B = - (|A| - |B|)
	{
		ret = SubC(C, A, B);
		(*C)->sign = 1;

		return SUCCESS;
	}
	else if (A_sign == 0 && B_sign == 1) // A > 0 , B < 0 �� ��� A - B = A + |B|
	{
		ret = bigint_gen(&tmp, 0, B->word_len, B->a);
		if (ret == FAIL)
		{
			printf("SUB �޸� �Ҵ� ����\n");
			return FAIL;
		}

		ret = Add(C, A, tmp);
		ret = bigint_free(&tmp);
		return SUCCESS;
	}
	else if (A_sign == 1 && B_sign == 0) // A < 0, B > 0 �� ��� A - B = -(|A| + B)
	{
		ret = bigint_gen(&tmp, 0, A->word_len, A->a);
		if (ret == FAIL)
		{
			printf("SUB �޸� �Ҵ� ����\n");
			return FAIL;
		}
		ret = Add(C, tmp, B);
		ret = bigint_free(&tmp);
		(*C)->sign = 1;

		return SUCCESS;
	}

	return FAIL;
}
















int SubC_k(bigint** C, bigint* A, bigint* B) // ���� �ƴ� ��ȣ�� �� ���� A,B(A >= B)�� ���� 
{

	unsigned int b = 0; // �������ų� ������ ���� ����
	int i = 0;
	int size = 0;
	WORD* Sub = NULL;
	int ret = 0;

	int word_len = 0;
	WORD W_1 = 0;


	if (WORD_bit_len == 8) // W�� �����ֱ� ���� �� ����. 
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

	if (B->word_len > A->word_len) // �� ���� A, B�� ������ ��� ������� �ִ� WORD_len�� max(A_word_len, B_word_len)�̴�.
	{							   // �� ���� �ƴ� ���� A(word_len = n) , B(word_len = m)�� ������ max(n, m)���� ������ �����Ͽ� ����Ѵ�.
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

		if (A->a[i] < b) // A->a[i] = 0, b = 1�� ���
		{
			Sub[i] = (W_1 + A->a[i]); // W_1 = W - 1. �� ���忡�� ���ڸ� ������ ���
			b = 1; // ������ �� = 1
		}
		else // A->a[i]�� 0�� �ƴ� ���
		{
			Sub[i] = A->a[i] - b;  // ������ ���� ���ش�.
			b = 0;
		}


		if (Sub[i] < B->a[i])  // �� ���忡�� �����;� �ϴ� ���
		{
			Sub[i] = (W_1 - B->a[i]) + Sub[i] + 1; // �� ���忡�� �������� ������ ���ش�.
			b = b + 1; // ������ ���� �����ش�.
		}
		else
		{
			Sub[i] = Sub[i] - B->a[i]; // ���� �ʿ䰡 ������ �׳� ������ ���ش�.
		}
	}


	
		ret = bigint_gen(C, 0, word_len, Sub);
		if(ret == FAIL)
		{
			printf("SUBC �޸� �Ҵ� ����\n");
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
		ret = bigint_gen(C, B->sign ^ 1, B->word_len, B->a); // B�� ��ȣ ����
		if (ret == FAIL)
		{
			printf("ADD �޸� �Ҵ� ����\n");
			return FAIL;
		}
		return SUCCESS;
	}
	else if (B_is_zero_val == zero) // B == 0, A - B = A
	{
		ret = bigint_gen(C, A->sign, A->word_len, A->a); // B�� ��ȣ ����
		if (ret == FAIL)
		{
			printf("ADD �޸� �Ҵ� ����\n");
			return FAIL;
		}
		return SUCCESS;
	}
	else if (A_sign == 0 && B_sign == 0 && (cmp_val == -1 || cmp_val == 0)) // A > 0, B > 0, A >= B�� ��� A - B = C
	{
		ret = SubC_k(C, A, B);

		return SUCCESS;
	}
	else if (A_sign == 0 && B_sign == 0 && cmp_val == 1) // A > 0 , B > 0, A < B �� ��� A - B = - (B - A)
	{
		ret = SubC_k(C, B, A);
		(*C)->sign = 1;


		return SUCCESS;
	}
	else if (A_sign == 1 && B_sign == 1 && (cmp_val == -1 || cmp_val == 0)) // A < 0, B < 0, A >= B �� ��� A - B = |B| - |A|
	{
		ret = SubC_k(C, B, A);

		return SUCCESS;
	}
	else if (A_sign == 1 && B_sign == 1 && cmp_val == 1) // A < 0, B < 0, A < B �� ��� A - B = - (|A| - |B|)
	{
		ret = SubC_k(C, A, B);
		(*C)->sign = 1;

		return SUCCESS;
	}
	else if (A_sign == 0 && B_sign == 1) // A > 0 , B < 0 �� ��� A - B = A + |B|
	{
		ret = bigint_gen(&tmp, 0, B->word_len, B->a);
		if (ret == FAIL)
		{
			printf("SUB �޸� �Ҵ� ����\n");
			return FAIL;
		}

		ret = Add(C, A, tmp);
		ret = bigint_free(&tmp);
		return SUCCESS;
	}
	else if (A_sign == 1 && B_sign == 0) // A < 0, B > 0 �� ��� A - B = -(|A| + B)
	{
		ret = bigint_gen(&tmp, 0, A->word_len, A->a);
		if (ret == FAIL)
		{
			printf("SUB �޸� �Ҵ� ����\n");
			return FAIL;
		}
		ret = Add(C, tmp, B);
		ret = bigint_free(&tmp);
		(*C)->sign = 1;

		return SUCCESS;
	}

	return FAIL;
}