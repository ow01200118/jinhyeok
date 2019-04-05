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
		num = (tmp_size % 2);	// �ֻ��� WORD �迭�� �� ������ string�� ����.  ex) tmp_size = 3, num = 1, w = 8, 0x1 0x2 0x3 string -> WORD[1] = 0x01, WORD[0] = 0x23 
		if (num == 1)	// �Է¹��� ������ ũ�Ⱑ WORD ������ �ƴ� ��
		{
			word_len = (tmp_size / 2) + 1; // ������ string�� WORD �迭�� ���� �ϹǷ� word_len�� 1 ������Ų��.
			(*A) = (WORD *)malloc(word_len * sizeof(WORD));
			memset((*A), 0x00, word_len * sizeof(WORD));
			for (i = 0; i < word_len - 1; i++) // �Է¹��� ���ڰ� WORD ������ ������ �Ѵ�. �ֻ��� WORD�迭���� ������ string�� ���� �ϹǷ� word_len - 1 ���� �ݺ����� �����Ѵ�.
			{
				hex[0] = asc_to_hex(tmp[tmp_size - 1]); // ex) 0x34�� �Է��Ͽ��ٸ� tmp[0] = '3', tmp[1] = '4'�� ���� ����Ǿ� �ִµ� asc_to_hex�� ���� hex[0] = 0x04, hex[1] = 0x03���� ��ȯ�Ͽ� ����
				hex[1] = asc_to_hex(tmp[tmp_size - 2]);
				(*A)[i] = (hex[1] << 4 | hex[0]); // ex) 0x34�� �Է��� ��� hex�� ��ȯ�� ���� WORD�迭�� 0x34�� �־�� �ϹǷ� shift�Ͽ� WORD�迭�� ����
				tmp_size = tmp_size - 2;
			}
			hex[2] = asc_to_hex(tmp[tmp_size - 1]); // WORD������ ���� �ʰ� ���� �Է¹��� ���ڵ��� �ֻ��� WORD �迭�� ����
			(*A)[i] = hex[2];
		}
		else // �Է¹��� ������ ũ�Ⱑ WORD������ ��
		{
			word_len = (tmp_size / 2); // ������ string�� ������ 0�̹Ƿ� ��Ȯ�� WORD������ ���� ���� �� �ִ�.
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
		num = (tmp_size % 8); // �ֻ��� WORD �迭�� �� ������ string�� ����
		if (num == 0)	// �Է¹��� ������ ũ�Ⱑ WORD ������ ��
		{
			word_len = (tmp_size / 8); // ������ string�� ������ 0�̹Ƿ� ��Ȯ�� WORD������ ���� ���� �� �ִ�
			(*A) = (WORD *)malloc(word_len * sizeof(WORD));
			memset((*A), 0x00, word_len * sizeof(WORD));
			for (i = 0; i < word_len; i++) // �Է¹��� string�� hex�� ��ȯ�Ͽ� WORD �迭�� ����
			{
				for (j = 0; j < 8; j++)
				{
					hex[j] = asc_to_hex(tmp[tmp_size - (j + 1)]);
					(*A)[i] |= (WORD)(hex[j]) << (4 * j);
				}
				tmp_size = tmp_size - 8;
			}
		}
		else // �Է¹��� ������ ũ�Ⱑ WORD ������ �ƴ� ��
		{
			word_len = (tmp_size / 8) + 1;
			(*A) = (WORD *)malloc(word_len * sizeof(WORD));
			memset((*A), 0x00, word_len * sizeof(WORD));
			for (i = 0; i < word_len - 1; i++) // �ֻ��� WORD�迭�� �����ϰ� ���� WORD�迭�� WORD������ ����
			{
				for (j = 0; j < 8; j++)
				{
					hex[j] = asc_to_hex(tmp[tmp_size - (j + 1)]);
					(*A)[i] |= (WORD)(hex[j]) << (4 * j);
				}
				tmp_size = tmp_size - 8;
			}

			for (k = num - 1; k >= 0; k--) // �ֻ��� WORD�迭�� ������ string ����
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