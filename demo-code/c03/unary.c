//Use this code as an i>clicker question
#include <stdio.h>
int main()
{
	int a, b, c;

	a = b = c = 1;
	printf("When a, b, c all have value 1,\n");
	printf("-a * b - c = %d\n", -a * b - c);
	printf("-a * -b - -c = %d\n", -a * -b - -c);
	printf("-(a * b - c) = %d\n", -(a * b - c));
	printf("Pay attention to the precendence of unary operater.\n"); 
	return 0;
}
