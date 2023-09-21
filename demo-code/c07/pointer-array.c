#include <stdio.h>
#include <stdlib.h>

int main()
{
	int a[10];
	int *p = a;

	int i;

	for(i=0; i<10; i++)
	{
		printf("%p\n", &a[i]);
	}
	printf("*****************\n");
	for(i=0; i<10; i++)
	{
		printf("%p\n", p+i);
	}

	printf("*****************\n");
	printf("sizeof(a) = %lu\n", sizeof(a));
	printf("sizeof(p) = %lu\n", sizeof(p));
	return 0;
}
