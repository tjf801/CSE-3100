#include <stdio.h>
#include <stdlib.h>

int main()
{
	int *p;

    	printf("The address of p (&p) is %p\n", &p);

	p = malloc(sizeof(int));
	if(p == NULL)
	{
		perror("Meomory allocation error.\n");
		return -1;
	}
	(*p) = 1;
	printf("The value of p is %p\n", p);
	printf("The value stored at the address pointed by p (*p) is %d\n", (*p));

	printf("The size of &p is %lu\n", sizeof(&p));
	printf("The size of p is %lu\n", sizeof(p));
	printf("The size of (*p) is %lu\n", sizeof((*p)));
	free(p);
	return 0;
}	
