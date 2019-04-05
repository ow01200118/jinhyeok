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

	if (rem == 0) // quo 워드 이동
	{
		src->a = (WORD *)realloc(src->a, (quo + src->word_len) * sizeof(WORD)); // (워드 길이 + quo) 만큼 메모리 재할당 
		memset(&(src->a[src->word_len]), 0x00, quo * sizeof(WORD)); // 재할당된 메모리 9으로 초기화
		memcpy(&(src->a[quo]), src->a, src->word_len * sizeof(WORD)); // quo만큼 워드 이동
		memset(src->a, 0x00, quo * sizeof(WORD)); // 이동된 워드를 0으로 초기화
		src->word_len += quo;

		return SUCCESS;
	}
	
	
	src->a = (WORD *)realloc(src->a, (src->word_len + quo + 1) * sizeof(WORD)); // quo 워드 이동 후 추가적으로 비트 이동 -> (결과값의 워드 길이 <= 워드 길이 + quo + 1)
	memset(&(src->a[src->word_len]), 0x00, (quo + 1) * sizeof(WORD));
	memcpy(&(src->a[quo]), src->a, src->word_len * sizeof(WORD));
	memset(src->a, 0x00, quo * sizeof(WORD));
	word_size = src->word_len + quo + 1;

	B = src->a[quo];
	src->a[quo] = (src->a[quo] << rem);

	for (i = quo + 1; i < word_size - 1; i++) // B는 이전 워드 저장, A는 현재 워드 저장
	{
		A = src->a[i];
		src->a[i] = (A << rem) | (B >> (WORD_bit_len - rem));
		B = A;
	}

	src->a[i] = B >> (WORD_bit_len - rem); // 최상위 워드
	src->word_len = word_size;
	if (src->a[i] == 0) // 최상위 워드가 0인 경우 워드 길이 1 감소 
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

	/*if (src->word_len == 1 && quo >= 1) //워드 길이가 1인데 1워드 이상 이동해야 할 때
	{
		src->a[0] = 0;
		return SUCCESS;
	}*/


	for (i = 0; i < src->word_len - quo; i++) // 워드 배수 만큼 이동
		src->a[i] = src->a[i + quo];


	src->a = (WORD *)realloc(src->a, (src->word_len - quo) * sizeof(WORD)); // 움직인 만큼 메모리 재할당
	src->word_len = src->word_len - quo; // 워드 길이 갱신


	if (rem != 0) // 워드 비트 길이 보다 작게 이동해야 하는 경우
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

	memset(&(src->a[quo + 1]), 0x00, src->word_len - quo - 1); // num 보다 큰 워드는 다 0으로 초기화
	src->a = (WORD *)realloc(src->a, (quo + 1) * sizeof(WORD)); // 워드 갱신
	src->word_len = quo + 1;

	if (rem != 0) // num이 워드 비트 길이의 배수가 아닐 때
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