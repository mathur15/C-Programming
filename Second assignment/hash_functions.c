#include <stdio.h>
#include <stdlib.h>
// Complete these two functions according to the assignment specifications
#include "hash.h"

#define BLOCK_SIZE 8

char *hash(FILE *f) {
	//intialize all the block size bytes to '\0'
	//read input using scanf and XOR(^) with corresponding byte in hash_val
	//continue the previous step until all bytes are read which means start again
	//from the first byte of hash_val
	int i;
	int count = 0;
	char input;
	char *hash_val = malloc(BLOCK_SIZE);
	for (i = 0;i < BLOCK_SIZE; i ++){
		hash_val [i] = '\0';
	}
	while (fread(&input, sizeof(char),1,f) != 0){
		//check the value of count first and then perform operations
		if(count == BLOCK_SIZE){
			count = 0;
		}
		//when count is less than or equal to block size
		hash_val[count] = hash_val[count] ^ input;
		count ++;
	}
	return hash_val;
}
	
