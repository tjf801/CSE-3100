#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void lastDigitCount(unsigned *a, unsigned n, unsigned freq[10]) {
	// it should NOT be the responsibility of a called function to initialize the caller's input variables.
	for (int i = 0; i < 10; ++i) freq[i] = 0;
	
	for (unsigned int i = 0; i < n; ++i) freq[a[i] % 10]++;
}

//Do not change the main function
int main(int argc, char *argv[]) {
	assert(argc == 2);
	unsigned n = atoi(argv[1]);
	
	assert(n >= 1 && n <= 10000);
	unsigned a[n], i, s = 0;
	
	for(i=0; i<n; i++) {
		s += 2*i+1;
		a[i] = s;
	}
	
	unsigned freq[10]; // any sane person would initialize this here as `... freq[10] = { 0 };`.
	lastDigitCount(a, n, freq);
	for(i=0; i<10; i++)
		printf("%d\t%d\n", i, freq[i]);
	
	return 0;
}
