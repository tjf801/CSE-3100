#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//TODO
void lastDigitCount(unsigned *a, unsigned n, unsigned freq[10])
{
	//fill in your code
}

//Do not change the main function
int main(int argc, char *argv[])
{
	assert(argc == 2);
	unsigned n = atoi(argv[1]);

	assert(n>=1 && n<=10000);
	unsigned a[n], i, s = 0;

	for(i=0; i<n; i++)
	{
		s += 2*i+1;
		a[i] = s;
	}
	unsigned freq[10]; 
	lastDigitCount(a, n, freq);
	for(i=0; i<10; i++)
		printf("%d\t%d\n", i, freq[i]);
	return 0;
}
