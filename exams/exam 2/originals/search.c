#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "run-search.h"

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		printf("Usage: %s n m\n", argv[0]);
		return 0;
	}
	int n = atoi(argv[1]);
	assert(n>=1 && n <=100000);
	int m = atoi(argv[2]);
	assert(m >= 2 && m <= 10*n);
	printf("n = %d, m = %d\n", n, m);
	run_search(n, m);
	return 0;
}
