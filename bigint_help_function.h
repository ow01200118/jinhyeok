#ifndef _HELP_H_
#define _HELP_H_

#include "bigint_define.h"

int string_to_array(WORD** A, unsigned char* tmp, unsigned int tmp_size); // �Է¹��� ���� string�� bigint ����ü�� WORD ������ ����� �Լ�
int set_sign(unsigned char* tmp, unsigned int* tmp_size); // �Է¹��� ���� string�� ��ȣ�� �Ǵ��ϴ� �Լ�
int compareABS(bigint* A, bigint* B); // ��ȣ�� ������ WORD���� ��Һ񱳸� �ϴ� �Լ�
int compare(bigint* A, bigint* B); // ��ȣ�� ������ WORD���� ����񱳸� �ϴ� �Լ�

int is_zero(bigint A); // �־��� WORD�� 0���� �Ǵ��ϴ� �Լ�


int is_one(bigint A); // �־��� WORD�� 0���� �Ǵ��ϴ� �Լ�

void show_array(bigint* A, char* array); // bigint ����ü�� ����ϴ� �Լ�
void swap_bignum(bigint** A, bigint** B);
unsigned char asc_to_hex(unsigned char A); // �Է¹��� ���� string�� 16���� ������ ��ȯ�ϴ� �Լ�

int left_shift(bigint* src, int num); // bigint�� left_shift �ϴ� �Լ�
int right_shift(bigint* src, int num); // bigint�� right_shift �ϴ� �Լ�
int mod(bigint* src, int num); // // bigint�� modular �ϴ� �Լ�

int bit_length(bigint* A); // bigint�� ��Ʈ ���̸� ���ϴ� �Լ�
WORD j_bit(bigint* A, int j); // bigint�� j-th bit�� ���ϴ� �Լ�

#endif