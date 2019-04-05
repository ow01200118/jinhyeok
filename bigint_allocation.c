#include "bigint_define.h"
#include "bigint_fucnction.h"
#include "bigint_help_function.h"


int bigint_gen(bigint** bignum, int sign, int word_len, WORD *tmp)
{

	int i = 0;
	int ret = 0;

	if ((*bignum) != NULL)
		ret = bigint_free(bignum); //Data initialize


	(*bignum) = (struct Bigint *)malloc(sizeof(struct Bigint)); // Make bigint structure

	if ((*bignum) == NULL) // Memory allocation fail
	{
		printf("memory allocation fail (bigint_gen)!!!\n");
		return FAIL;
	}
		
	memset(*bignum, 0x00, sizeof(struct Bigint)); // Initialize the value of allocated memory to 0

	(*bignum)->word_len = word_len; // Input a bigint of word length
	(*bignum)->sign = sign; // Input a bigint of sign
	(*bignum)->a = (WORD *)malloc(((*bignum)->word_len) * sizeof(WORD)); // Make bigint array

	if ((*bignum)->a == NULL) // Memory allocation fail
	{
		printf("memory allocation fail (bigint_gen)!!!\n");
		return FAIL;
	}

	memset((*bignum)->a, 0x00, ((*bignum)->word_len) * sizeof(WORD)); // Initialize the value of allocated memory to 0

	memcpy((*bignum)->a, tmp, ((*bignum)->word_len) * sizeof(WORD)); // Input value of bignumber

	

	return SUCCESS;

}

int bigint_free(bigint** bignum)
{

	int i = 0;
	if ((*bignum) == NULL) // Already initialized 
		return SUCCESS;

	memset((*bignum)->a, 0x00, (*bignum)->word_len); // Initialize the value of bignumber to 0
	free((*bignum)->a);
	(*bignum)->a = NULL;

	memset((*bignum), 0x00, 12); // Initialize the value of bigint structure to 0
	free(*bignum); 
	(*bignum) = NULL;

	return SUCCESS;

}