#include "bigint_define.h"
#include "bigint_fucnction.h"
#include "bigint_help_function.h"








int set_sign(unsigned char* tmp, unsigned int* tmp_size)
{
	int sign = 0;
	if (tmp[0] == 0x2D) // �Է¹��� string�� ù ���ڰ� '-' �� ��� sign = 1�� �����ϰ� string���� '-' ����
	{
		sign = 1;
		memcpy(tmp, &tmp[1], tmp_size[0] - 1);
		tmp[tmp_size[0] - 1] = 0x00;
		tmp_size[0]--;
	}

	return sign;
}





int compareABS(bigint* A, bigint* B)
{
	int A_word_len = 0;
	int B_word_len = 0;
	int i = 0;

	A_word_len = A->word_len; // A�� word_len ����
	B_word_len = B->word_len; // B�� word_len ����


	if (A_word_len > B_word_len) // word_len�� ũ�� ������ ũ�⵵ ũ��.
	{
		return 0; // A > B
	}
	else if (A_word_len < B_word_len)
	{
		return 1; // A < B
	}
	else // word_len�� ���� ��� ���� WORD���� �� ���ڸ� ��
	{
		for (i = A_word_len - 1; i >= 0; i--)
		{
			if (A->a[i] > B->a[i])
			{
				return 0; // A > B
			}
			else if (A->a[i] < B->a[i])
			{
				return 1; // A < B
			}
		}
		return -1; // A = B
	}

}

int compare(bigint* A, bigint* B)
{
	int val = 0; // compareABS �Լ��� ������� ������ ����



	if (A->sign == 0 && B->sign == 1) // +A > -B 
	{
		return 0; // A > B
	}
	else if (A->sign == 1 && B->sign == 0) // -A < +B
	{
		return 1; // A < B
	}
	else // �� ���� ��ȣ�� ���� ���
	{

		val = compareABS(A, B);

		if (val == -1) // ��ȣ�� ������ compareABS�� ������� A = B�� ���
		{
			return -1; // A = B
		}
		else if (A->sign == 0) // ��ȣ�� +�� ��� compareABS�� ������� ����
		{
			return val;
		}
		else // ��ȣ�� -�� ��� compareABS�� ������� �ݴ� ex) A = -1, B = -2 -> |A| < |B| -> A > B
		{

			return (val ^ 1);
		}
	}
}


int is_zero(bigint A)
{
	int i = 0;

	for (i = A.word_len - 1; i >= 0; i--)
	{
		if (A.a[i] != 0)
		{
			return nonzero;
		}
	}

	return zero;
}


int is_one(bigint A)
{
	int i = 0;

	for (i = A.word_len - 1; i >= 1; i--)
	{
		if (A.a[i] != 0)
		{
			return nonone;
		}
	}

	if (A.a[i] == 1)
	{
		return one;
	}

	return nonone;
}



void show_array(bigint* A, char* array)
{
	int i = 0;

	printf("%s= ", array);
	if (A->sign == 1)
	{
		printf("-");
	}

	printf("0x");

	for (i = A->word_len - 1; i >= 0; i--)
	{
#if	(WORD_bit_len == 8)
{
	printf("%02x", A->a[i]);
}
#elif (WORD_bit_len == 32)		
{
	printf("%08x", A->a[i]);
}
#elif (WORD_bit_len == 64)	
{
	printf("%016llx", A->a[i]);
}
#endif
	}
	printf("\n\n");

}

void swap_bignum(bigint** A, bigint** B)
{
	bigint* tmp = NULL;

	tmp = *A;
	*A = *B;
	*B = tmp;
}


int bit_length(bigint* A)
{
	WORD MSW = 0;
	int ret = 0;
	WORD tmp = 0;
	int i = 0;

	MSW = A->a[A->word_len - 1];

	for (i = WORD_bit_len - 1; i >= 0; i--)
	{
		tmp = (MSW >> i);
		if (tmp == 1)
			break;
	}

	//ret = (int)(log((double)MSW) / log(2.0)) + 1;
	ret = ((A->word_len) - 1) * WORD_bit_len + i + 1;
	return ret;

}
WORD j_bit(bigint* A, int j)
{
	int quo = 0;
	int rem = 0;
	int ret = 0;
	WORD W = 0;
	WORD tmp = 1;
	quo = j / WORD_bit_len;
	rem = j % WORD_bit_len;


	W = A->a[quo];
	W = W >> rem;
	W = W & tmp;

	if (W == 0)
		return (WORD)0;

	return (WORD)1;

	/*if (rem == 0)
	{
	W = A->a[quo - 1];
	W = W >> (WORD_bit_len - 1);
	W = W & tmp;

	if (W == 0)
	return 0;

	return (WORD)1;
	}

	W = A->a[quo];
	W = W >> (rem - 1);
	W = W & tmp;

	if (W == 0)
	return (WORD)0;

	return (WORD)1;*/
}

