#include <stdio.h>
#include <stdlib.h>

int main()
{
	int   a[10];
	int   *p = a;

	printf("%p\n", a);
	printf("%p\n", p);
	printf("%p\n", (&a)[1]);
	printf("%p\n", (&p)[1]);
	return 0;
 
}
