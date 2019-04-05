#include "bigint_define.h"
#include "bigint_fucnction.h"
#include "bigint_help_function.h"



int main()
{

	unsigned char tmp[65536] = { 0x00, };
	unsigned char tmp1[65536] = { 0x00, };
	unsigned char tmp2[65536] = { 0x00, };
	unsigned char tmp3[65536] = { 0x00, };
	unsigned int tmp_size = 0;
	unsigned int tmp1_size = 0;
	unsigned int tmp2_size = 0;
	unsigned int tmp3_size = 0;
	int word_len_A = 0;
	int sign_A = 0;
	int word_len_B = 0;
	int sign_B = 0;
	int word_len_C = 0;
	int word_len_D = 0;
	int val = 0;
	int ret = 0;
	int i = 0;
	bigint* Red_num = NULL;
	bigint* Exp_num = NULL;
	bigint* A = NULL;
	bigint* B = NULL;
	bigint* Add_result = NULL;
	bigint* Sub_result = NULL;
	bigint* Mul_result = NULL;
	bigint* Mul_result_k = NULL;
	bigint* Squ_result_A = NULL;
	bigint* Squ_result_B = NULL;
	bigint* Squ_result_Ak = NULL;
	bigint* Squ_result_Bk = NULL;
	bigint* Exp_result = NULL;
	bigint* Red_result = NULL;
	bigint* quo = NULL;
	bigint* rem = NULL;
	WORD* A_word = NULL;
	WORD* B_word = NULL;
	WORD* C_word = NULL;
	WORD* D_word = NULL;






	printf("Input numA(hex) : 0x");
	scanf("%s", tmp);
	printf("Input Positive numB(hex) : 0x");
	scanf("%s", tmp1);
	printf("Positive Exponentiation num(hex) : 0x");
	scanf("%s", tmp2);
	printf("Positive Odd Reduction num(hex) : 0x");
	scanf("%s", tmp3);

	tmp_size = (unsigned int)strlen(tmp);
	tmp1_size = (unsigned int)strlen(tmp1);
	tmp2_size = (unsigned int)strlen(tmp2);
	tmp3_size = (unsigned int)strlen(tmp3);
	sign_A = set_sign(tmp, &tmp_size);
	sign_B = set_sign(tmp1, &tmp1_size);
	word_len_A = string_to_array(&A_word, tmp, tmp_size);
	word_len_B = string_to_array(&B_word, tmp1, tmp1_size);
	word_len_C = string_to_array(&C_word, tmp2, tmp2_size);
	word_len_D = string_to_array(&D_word, tmp3, tmp3_size);

	ret = bigint_gen(&A, sign_A, word_len_A, A_word);
	if (ret == FAIL)
		printf("memory allocation fail (main)!!!\n");

	memset(A_word, 0x00, word_len_A * sizeof(WORD));
	free(A_word);
	A_word = NULL;


	ret = bigint_gen(&B, sign_B, word_len_B, B_word);
	if (ret == FAIL)
		printf("memory allocation fail (main)!!!\n");


	memset(B_word, 0x00, word_len_B * sizeof(WORD));
	free(B_word);
	B_word = NULL;

	ret = bigint_gen(&Exp_num, 0, word_len_C, C_word);
	if (ret == FAIL)
		printf("memory allocation fail (main)!!!\n");

	memset(C_word, 0x00, word_len_C * sizeof(WORD));
	free(C_word);
	C_word = NULL;

	ret = bigint_gen(&Red_num, 0, word_len_D, D_word);
	if (ret == FAIL)
		printf("memory allocation fail (main)!!!\n");

	memset(D_word, 0x00, word_len_D * sizeof(WORD));
	free(D_word);
	D_word = NULL;

	printf("\n");
	show_array(A, "A");
	show_array(B, "B");











	printf("**********************************Addtion***********************************************\n");
	show_array(A, "A");
	show_array(B, "B");
	ret = Add(&Add_result, A, B);
	show_array(Add_result, "A + B =");
	bigint_free(&Add_result);
	ret = Add(&Add_result, A, B);
	bigint_free(&Add_result);



	printf("**********************************Subtraction***********************************************\n");
	show_array(A, "A");
	show_array(B, "B");
	ret = Sub(&Sub_result, A, B);
	show_array(Sub_result, "A - B =");
	bigint_free(&Sub_result);



	printf("**********************************Multiplication (School book)***********************************************\n");
	show_array(A, "A");
	show_array(B, "B");
	Mul(&Mul_result, A, B, 's');
	show_array(Mul_result, "A * B =");
	bigint_free(&Mul_result);




	printf("**********************************Multiplication (Karatsuba)***********************************************\n");
	show_array(A, "A");
	show_array(B, "B");
	Mul(&Mul_result_k, A, B, 'k');
	show_array(Mul_result_k, "A * B =");
	bigint_free(&Mul_result_k);


	printf("**********************************A square (School book)***********************************************\n");
	show_array(A, "A");
	show_array(B, "B");
	Squ(&Squ_result_A, A, 's');
	show_array(Squ_result_A, "A^2 =");
	bigint_free(&Squ_result_A);


	printf("**********************************B square (School book)***********************************************\n");
	show_array(A, "A");
	show_array(B, "B");
	Squ(&Squ_result_B, B, 's');
	show_array(Squ_result_B, "B^2 =");
	bigint_free(&Squ_result_B);


	printf("**********************************A square (Karatsuba)***********************************************\n");
	show_array(A, "A");
	show_array(B, "B");
	Squ(&Squ_result_Ak, A, 'k');
	show_array(Squ_result_Ak, "A^2 =");
	bigint_free(&Squ_result_Ak);


	printf("**********************************B square (Karatsuba)***********************************************\n");
	show_array(A, "A");
	show_array(B, "B");
	Squ(&Squ_result_Bk, B, 'k');
	show_array(Squ_result_Bk, "B^2 =");
	bigint_free(&Squ_result_Bk);

	printf("**********************************long division (bit-version)***********************************************\n");
	show_array(A, "A");
	show_array(B, "B");
	long_div_bit(A, B, &quo, &rem);
	show_array(quo, "A//B =");
	show_array(A, "A");
	show_array(B, "B");
	show_array(rem, "A%B =");
	bigint_free(&quo);
	bigint_free(&rem);

	printf("**********************************long division (word-version)***********************************************\n");
	show_array(A, "A");
	show_array(B, "B");
	long_div_word(A, B, &quo, &rem);
	show_array(quo, "A//B =");
	show_array(A, "A");
	show_array(B, "B");
	show_array(rem, "A%B =");
	bigint_free(&quo);
	bigint_free(&rem);

	/***********************************************************************************************************************************************************************************************************
	printf("**********************************Left-to-Right Exponentiation***********************************************\n");
	show_array(A, "A");
	show_array(Exp_num, "e");
	L_to_R(&Exp_result, A, Exp_num);
	show_array(Exp_result, "A ^ e =");
	bigint_free(&Exp_result);

	printf("**********************************Right-to-Left Exponentiation***********************************************\n");
	show_array(A, "A");
	show_array(Exp_num, "e");
	R_to_L(&Exp_result, A, Exp_num);
	show_array(Exp_result, "A ^ e =");
	bigint_free(&Exp_result);

	printf("**********************************Montgomery Exponentiation***********************************************\n");
	show_array(A, "A");
	show_array(Exp_num, "e");
	MontExp(&Exp_result, A, Exp_num);
	show_array(Exp_result, "A ^ e =");
	bigint_free(&Exp_result);
	**********************************************************************************************************************************************************************************************************/

	printf("**********************************Barret Reduction***********************************************\n");
	show_array(A, "A");
	show_array(Red_num, "N");
	BarretRed(&Red_result, A, Red_num);
	show_array(Red_result, "A % N =");
	bigint_free(&Red_result);

	printf("**********************************Modular Exponentiation using Montgomery Reduction***********************************************\n");
	bigint* R = NULL;
	bigint* X = NULL;
	bigint* Y = NULL;
	bigint* R_inv = NULL;
	bigint* N_inv = NULL;
	bigint* x_e = NULL;
	bigint* x_e_tmp = NULL;
	MontGetR(Red_num, &R);
	EEA(&X, &Y, R, Red_num);
	if (X->sign == 1)
		Add(&R_inv, X, Red_num);
	else
		bigint_gen(&R_inv, X->sign, X->word_len, X->a);

	if (Y->sign == 1)
		Add(&N_inv, Y, R);
	else
		bigint_gen(&N_inv, Y->sign, Y->word_len, Y->a);

	bigint_free(&X);
	bigint_free(&Y);

	MontExpRed(&x_e, A, Red_num, R, N_inv, Exp_num);
	if (x_e->sign == 1)
	{
		Add(&x_e_tmp, x_e, Red_num);
		swap_bignum(&x_e_tmp, &x_e);
		bigint_free(&x_e_tmp);
	}
	show_array(A, "A");      printf("\n");
	show_array(Exp_num, "e");      printf("\n");
	show_array(Red_num, "N");      printf("\n");
	show_array(R, "R");      printf("\n");
	show_array(x_e, "(A^e) % N =");      printf("\n");

	bigint_free(&R_inv);
	bigint_free(&N_inv);
	bigint_free(&x_e);
	bigint_free(&x_e_tmp);
	bigint_free(&R);

	bigint_free(&A);
	bigint_free(&B);
	bigint_free(&Red_num);
	bigint_free(&Exp_num);
}




