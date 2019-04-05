#ifndef _HELP_H_
#define _HELP_H_

#include "bigint_define.h"

int string_to_array(WORD** A, unsigned char* tmp, unsigned int tmp_size); // 입력받은 숫자 string을 bigint 구조체에 WORD 단위로 만드는 함수
int set_sign(unsigned char* tmp, unsigned int* tmp_size); // 입력받은 숫자 string의 부호를 판단하는 함수
int compareABS(bigint* A, bigint* B); // 부호를 제외한 WORD들의 대소비교를 하는 함수
int compare(bigint* A, bigint* B); // 부호를 포함한 WORD들의 대수비교를 하는 함수

int is_zero(bigint A); // 주어진 WORD가 0인지 판단하는 함수


int is_one(bigint A); // 주어진 WORD가 0인지 판단하는 함수

void show_array(bigint* A, char* array); // bigint 구조체를 출력하는 함수
void swap_bignum(bigint** A, bigint** B);
unsigned char asc_to_hex(unsigned char A); // 입력받은 숫자 string을 16진수 값으로 변환하는 함수

int left_shift(bigint* src, int num); // bigint를 left_shift 하는 함수
int right_shift(bigint* src, int num); // bigint를 right_shift 하는 함수
int mod(bigint* src, int num); // // bigint를 modular 하는 함수

int bit_length(bigint* A); // bigint의 비트 길이를 구하는 함수
WORD j_bit(bigint* A, int j); // bigint의 j-th bit를 구하는 함수

#endif