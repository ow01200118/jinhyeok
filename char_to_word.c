#include "bigint_define.h"
#include "bigint_fucnction.h"
#include "bigint_help_function.h"


unsigned char asc_to_hex(unsigned char A)
{

	if (A >= 48 && A <= 57)			// string 0~9
		A = (A - 48);				// string 0~9 -> hex 0~9
	else if (A >= 65 && A <= 70)	// string A~F 
		A = (A - 55);				// string A~F -> hex a~f
	else if (A >= 97 && A <= 102)	// string a~f
		A = (A - 87);				// string a~f -> hex a~f

	return A;
}

int string_to_array(WORD** A, unsigned char* tmp, unsigned int tmp_size)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int z = 0;
	int num = 0;
	int word_len = 0;
	unsigned char hex[17] = { 0x00, };
	

	if (WORD_bit_len == 8)	// w = 8
	{
		num = (tmp_size % 2);	// 최상위 WORD 배열에 들어갈 나머지 string의 개수.  ex) tmp_size = 3, num = 1, w = 8, 0x1 0x2 0x3 string -> WORD[1] = 0x01, WORD[0] = 0x23 
		if (num == 1)	// 입력받은 숫자의 크기가 WORD 단위가 아닐 때
		{
			word_len = (tmp_size / 2) + 1; // 나머지 string이 WORD 배열에 들어가야 하므로 word_len를 1 증가시킨다.
			(*A) = (WORD *)malloc(word_len * sizeof(WORD));
			memset((*A), 0x00, word_len * sizeof(WORD));
			for (i = 0; i < word_len - 1; i++) // 입력받은 숫자가 WORD 단위로 들어가도록 한다. 최상위 WORD배열에는 나머지 string이 들어가야 하므로 word_len - 1 까지 반복문을 실행한다.
			{
				hex[0] = asc_to_hex(tmp[tmp_size - 1]); // ex) 0x34를 입력하였다면 tmp[0] = '3', tmp[1] = '4'의 값이 저장되어 있는데 asc_to_hex를 통해 hex[0] = 0x04, hex[1] = 0x03으로 변환하여 저장
				hex[1] = asc_to_hex(tmp[tmp_size - 2]);
				(*A)[i] = (hex[1] << 4 | hex[0]); // ex) 0x34를 입력한 경우 hex로 변환한 값을 WORD배열에 0x34로 넣어야 하므로 shift하여 WORD배열에 저장
				tmp_size = tmp_size - 2;
			}
			hex[2] = asc_to_hex(tmp[tmp_size - 1]); // WORD단위로 들어가지 않고 남은 입력받은 숫자들을 최상위 WORD 배열에 저장
			(*A)[i] = hex[2];
		}
		else // 입력받은 숫자의 크기가 WORD단위일 때
		{
			word_len = (tmp_size / 2); // 나머지 string의 개수가 0이므로 정확히 WORD단위로 값을 넣을 수 있다.
			(*A) = (WORD *)malloc(word_len * sizeof(WORD));
			memset((*A), 0x00, word_len * sizeof(WORD));
			for (i = 0; i < word_len; i++)
			{
				hex[0] = asc_to_hex(tmp[tmp_size - 1]);
				hex[1] = asc_to_hex(tmp[tmp_size - 2]);
				(*A)[i] = (hex[1] << 4 | hex[0]);
				tmp_size = tmp_size - 2;
			}
		}
	}
	else if (WORD_bit_len == 32) // w = 32
	{
		num = (tmp_size % 8); // 최상위 WORD 배열에 들어갈 나머지 string의 개수
		if (num == 0)	// 입력받은 숫자의 크기가 WORD 단위일 때
		{
			word_len = (tmp_size / 8); // 나머지 string의 개수가 0이므로 정확히 WORD단위로 값을 넣을 수 있다
			(*A) = (WORD *)malloc(word_len * sizeof(WORD));
			memset((*A), 0x00, word_len * sizeof(WORD));
			for (i = 0; i < word_len; i++) // 입력받은 string을 hex로 변환하여 WORD 배열에 저장
			{
				for (j = 0; j < 8; j++)
				{
					hex[j] = asc_to_hex(tmp[tmp_size - (j + 1)]);
					(*A)[i] |= (WORD)(hex[j]) << (4 * j);
				}
				tmp_size = tmp_size - 8;
			}
		}
		else // 입력받은 숫자의 크기가 WORD 단위가 아닐 때
		{
			word_len = (tmp_size / 8) + 1;
			(*A) = (WORD *)malloc(word_len * sizeof(WORD));
			memset((*A), 0x00, word_len * sizeof(WORD));
			for (i = 0; i < word_len - 1; i++) // 최상위 WORD배열을 제외하고 이전 WORD배열에 WORD단위로 저장
			{
				for (j = 0; j < 8; j++)
				{
					hex[j] = asc_to_hex(tmp[tmp_size - (j + 1)]);
					(*A)[i] |= (WORD)(hex[j]) << (4 * j);
				}
				tmp_size = tmp_size - 8;
			}

			for (k = num - 1; k >= 0; k--) // 최상위 WORD배열에 나머지 string 저장
			{
				hex[z] = asc_to_hex(tmp[k]);
				(*A)[i] |= (WORD)(hex[z]) << (4 * z);
				z++;
			}
		}
	}
	else if (WORD_bit_len == 64)
	{
		num = (tmp_size % 16);
		if (num == 0)
		{
			word_len = (tmp_size / 16);
			(*A) = (WORD *)malloc(word_len * sizeof(WORD));
			memset((*A), 0x00, word_len * sizeof(WORD));
			for (i = 0; i < word_len; i++)
			{
				for (j = 0; j < 16; j++)
				{
					hex[j] = asc_to_hex(tmp[tmp_size - (j + 1)]);
					(*A)[i] |= (WORD)(hex[j]) << (4 * j);
				}
				tmp_size = tmp_size - 16;
			}
		}
		else
		{
			word_len = (tmp_size / 16) + 1;
			(*A) = (WORD *)malloc(word_len * sizeof(WORD));
			memset((*A), 0x00, word_len * sizeof(WORD));
			for (i = 0; i < word_len - 1; i++)
			{
				for (j = 0; j < 16; j++)
				{
					hex[j] = asc_to_hex(tmp[tmp_size - (j + 1)]);
					(*A)[i] |= (WORD)(hex[j]) << (4 * j);
				}
				tmp_size = tmp_size - 16;
			}

			for (k = num - 1; k >= 0; k--)
			{
				hex[z] = asc_to_hex(tmp[k]);
				(*A)[i] |= (WORD)(hex[z]) << (4 * z);
				z++;
			}
		}
	}

	return word_len;

}