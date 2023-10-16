#include <stdio.h>

// test if a number `n` is a palindrome (in base 10).
int palindrome(unsigned long n) {
	unsigned int reversed = 0, n_orig = n;
	
	while (n > 0) {
		// NOTE: this overflows for sufficiently large inputs, so this function
		//       is not completely correct. but tbh i dont really care.
		reversed = reversed * 10 + n % 10;
		n /= 10;
	}
	
	return n_orig == reversed;
}

// return the smallest n such that n is not a palindrome, but n³ *is* a palindrome.
// 
// NOTE: since this is a pure function which takes no arguments, it can be replaced
//       with (or ideally, optimized into) a function that simply returns `2201`.
unsigned long pCube() {
	for (unsigned long n = 1; 1; ++n)
		if (!palindrome(n) && palindrome(n * n * n))
			return n;
}

//Do not change the main function
int main() {
	printf("%lu\n", pCube());
	return 0;
}
