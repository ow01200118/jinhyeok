#include "bigint_define.h"
#include "bigint_fucnction.h"
#include "bigint_help_function.h"

int left_shift(bigint* src, int num)
{

	unsigned int quo = 0;
	unsigned int rem = 0;
	WORD A = 0;
	WORD B = 0;
	int i = 0;
	int ret = 0;
	int word_size = 0;



	/*ret = is_zero(*src);
	if (ret == zero)
		return SUCCESS;*/
	if (num == 0)
		return SUCCESS;

	quo = num / WORD_bit_len;
	rem = num % WORD_bit_len;

	word_size = src->word_len;

	if (rem == 0) // quo ���� �̵�
	{
		src->a = (WORD *)realloc(src->a, (quo + src->word_len) * sizeof(WORD)); // (���� ���� + quo) ��ŭ �޸� ���Ҵ� 
		memset(&(src->a[src->word_len]), 0x00, quo * sizeof(WORD)); // ���Ҵ�� �޸� 9���� �ʱ�ȭ
		memcpy(&(src->a[quo]), src->a, src->word_len * sizeof(WORD)); // quo��ŭ ���� �̵�
		memset(src->a, 0x00, quo * sizeof(WORD)); // �̵��� ���带 0���� �ʱ�ȭ
		src->word_len += quo;

		return SUCCESS;
	}
	
	
	src->a = (WORD *)realloc(src->a, (src->word_len + quo + 1) * sizeof(WORD)); // quo ���� �̵� �� �߰������� ��Ʈ �̵� -> (������� ���� ���� <= ���� ���� + quo + 1)
	memset(&(src->a[src->word_len]), 0x00, (quo + 1) * sizeof(WORD));
	memcpy(&(src->a[quo]), src->a, src->word_len * sizeof(WORD));
	memset(src->a, 0x00, quo * sizeof(WORD));
	word_size = src->word_len + quo + 1;

	B = src->a[quo];
	src->a[quo] = (src->a[quo] << rem);

	for (i = quo + 1; i < word_size - 1; i++) // B�� ���� ���� ����, A�� ���� ���� ����
	{
		A = src->a[i];
		src->a[i] = (A << rem) | (B >> (WORD_bit_len - rem));
		B = A;
	}

	src->a[i] = B >> (WORD_bit_len - rem); // �ֻ��� ����
	src->word_len = word_size;
	if (src->a[i] == 0) // �ֻ��� ���尡 0�� ��� ���� ���� 1 ���� 
	{
		src->word_len--;
		src->a = (WORD *)realloc(src->a, (src->word_len) * sizeof(WORD));
		
	}


	

	return SUCCESS;

}

int	right_shift(bigint* src, int num)
{
	
	int quo = 0;
	int rem = 0;
	WORD A = 0;
	WORD B = 0;
	int i = 0;
	int ret = 0;
	int word_size = 0;
	WORD zero_init = 0x00;



	if (num == 0)
		return SUCCESS;

	quo = num / WORD_bit_len;
	rem = num % WORD_bit_len;

	word_size = src->word_len;

	if (src->word_len <= quo)
	{
		
		src->a = (WORD *)realloc(src->a, sizeof(WORD));
		src->a[0] = 0x00;
		src->sign = 0;
		src->word_len = 1;
		return SUCCESS;
	}

	/*if (src->word_len == 1 && quo >= 1) //���� ���̰� 1�ε� 1���� �̻� �̵��ؾ� �� ��
	{
		src->a[0] = 0;
		return SUCCESS;
	}*/


	for (i = 0; i < src->word_len - quo; i++) // ���� ��� ��ŭ �̵�
		src->a[i] = src->a[i + quo];


	src->a = (WORD *)realloc(src->a, (src->word_len - quo) * sizeof(WORD)); // ������ ��ŭ �޸� ���Ҵ�
	src->word_len = src->word_len - quo; // ���� ���� ����


	if (rem != 0) // ���� ��Ʈ ���� ���� �۰� �̵��ؾ� �ϴ� ���
	{
		for (i = 0; i < src->word_len - 1; i++)
			src->a[i] = src->a[i + 1] << (WORD_bit_len - rem) | (src->a[i] >> rem);

		src->a[i] = src->a[i] >> rem;

		if (src->a[i] == 0 && src->word_len != 1)
		{
			src->word_len--;
			src->a = (WORD *)realloc(src->a, (src->word_len) * sizeof(WORD));
			
		}

	}

	return SUCCESS;
}

int mod(bigint* src, int num)
{

	int quo = 0;
	int rem = 0;

	WORD A = 0;


	if (num == 0)
		return SUCCESS;
	if (src->word_len * WORD_bit_len <= num)
		return SUCCESS;

	quo = num / WORD_bit_len;
	rem = num % WORD_bit_len;

	memset(&(src->a[quo + 1]), 0x00, src->word_len - quo - 1); // num ���� ū ����� �� 0���� �ʱ�ȭ
	src->a = (WORD *)realloc(src->a, (quo + 1) * sizeof(WORD)); // ���� ����
	src->word_len = quo + 1;

	if (rem != 0) // num�� ���� ��Ʈ ������ ����� �ƴ� ��
		src->a[quo] = (((WORD)(1) << rem) - 1) & src->a[quo];
	else
		src->a[quo] = 0;

	if (src->a[quo] == 0)
	{
		src->word_len--;
		src->a = (WORD *)realloc(src->a, src->word_len * sizeof(WORD));
	}

	return SUCCESS;
}