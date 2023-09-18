//In this assignment, we wirte code to sum up the values of the first n bytes in a memory block
//that is allocated for n unsigned integers.
//The memory allocation occurs in the main function
//We need to implement the function sum_n_bytes
//This function returns the sum of the values of the first n bytes that is pointed by 
//the pointer p

 
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

//Implement the following function
//The function sum_n_bytes returns the sum of the values of the first n bytes in the memory block
//pointed by the pointer p
size_t sum_n_bytes(const uint8_t * const restrict p, const size_t n)
{
	//fill in your code below
	//Note that the sum should be an unsigned integer
	//This should has some ramifications on your code
	//Only a few lines of code expected
	size_t sum = 0;
	for (size_t i = 0; i < n; ++i) sum += p[i];
	return sum;
}

//Note how we use the command line arguments
int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("Usage: %s n\n", argv[0]);
		exit(-1);
	}
	int n = atoi(argv[1]);
	//allocate memory for n unsigned integers
	unsigned *p = calloc(n, sizeof(unsigned));
	//Assign values to these unsigned integers in a certain pattern
	int i;
	for(i=0; i< n; i++)
	{
		p[i] = 1 << (i % 32);
	}
	//print out the sum of the first n bytes
	printf("%d\n", sum_n_bytes(p, n));
	//do not forget to free p
	free(p);
	return 0;
}
