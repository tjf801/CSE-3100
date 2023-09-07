//In this assignment, we will find the smallest positive integer that 
// can be expressed as a sum of two positive cube numbers in two distinct ways.
// More specifically, we want to find the smallest n such that n == i1*i1*i1 + j1*j1*j1, 
// n == i2*i2*i2 + j2*j2*j2, and (i1, j1) and (i2, j2) are not the same in the sense that
// not only (i1, j1) not euqal to (i2, j2), but also (i1, j1) not equal to (j2, i2).
// We practice using loops in this assignment.
// Also, we practice how to do an early exit in a loop. We exit the loop once we find the smallest n. 
// Once we are done, we will be delighted surprised to find out that this number already played a big role in 
// our computer science study.
#include <stdio.h>

// NOTE: yes, i know this is overkill. but if my algorithm cant check all numbers
// in https://oeis.org/A001235 below a hundred million in ~three minutes, then i
// might as well just use python or something.
_Bool is_cube(int n) {
	if (n < 0) n = -n;
	if (n == 1) return 1;
	
	// i love premature optimization
	switch (n % 7) {
		case 0: case 1: case 6: break;
		default: return 0;
	}
	switch (n % 13) {
		case 0: case 1: case 5: case 8: case 12: break;
		default: return 0;
	}
	
	// newton's method
	// needs to be ULL to avoid "Floating point exception" with larger values of n
	unsigned long long x = n / 2;
	while (!(x * x * x <= n && n < (x+1) * (x+1) * (x+1))) {
		x = (n / (x * x) + 2 * x) / 3;
	}
	return x * x * x == n;
}

_Bool is_taxicab_number(int n) {
	_Bool found_first_sum = 0;
	
	for (int i = 1; i * i * i < n; ++i) {
		int maybe_cube = n - (i * i * i);
		
		if (is_cube(maybe_cube) && maybe_cube > i * i * i) {
			if (found_first_sum) return 1;
			found_first_sum = 1;
		}
	}
	
	return 0;
}

int main()
{
	int i, j, n;

	//We assume the smallest such n is no more than 1000000
	for(n=1; n<=1000000; n++)
		if (is_taxicab_number(n)) break;
	
	//Do not change code below
    printf("%d is the solution:\n", n);
	for(i=1; i<=100; i++)
		for(j = i; j<= 100; j++)
			{
				if (i*i*i + j*j*j == n)
				{
					printf("%d * %d * %d + %d * %d * %d = %d\n", i, i, i, j, j, j, n);
                }
			}
	
	return 0;
	//Do not try to hard code the solution, that is not the way to learn.
}
