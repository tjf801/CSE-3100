#include <stdio.h>
#include <stdlib.h>

int main()
{
	int *p, n = 1;

	p = malloc(sizeof(int));
	if(p==NULL)
	{
		perror("Cannot allocate memory!\n");
		exit(-1);
	}
	printf("%p\n", p);
	(*p) = 0;
	printf("%d\n", *p);

	*p+=n;
	printf("%p\n", p);
	printf("%d\n", *p);

	free(p);

	
	return 0;
}
