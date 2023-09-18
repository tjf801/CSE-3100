#include <stdio.h>
#include <stdlib.h>

int main()
{
	int *p;

	printf("%p\n", p);
	(*p) = 0;
	printf("%d\n", *p);
	return 0;
}
