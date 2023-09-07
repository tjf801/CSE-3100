#include <stdio.h>
#include <stdlib.h>

int main()
{
	int n, i;
	
	printf("n = ");
	scanf("%d", &n);
	
	double pi = 0.;
	
	for (i = 0; i <= n; ++i) {
		pi += (4./(8*i + 1) - 2./(8*i + 4) - 1./(8*i + 5) - 1./(8*i + 6)) / (1ull << (4 * i));
	}
	
	printf("PI = %.10f\n", pi);
	return 0;
}
