#include <stdio.h>
#include <stdlib.h>

int main()
{
	int *p;

	p = malloc(sizeof(int));
	if(p==NULL)
	{
		perror("Cannot allocate memory!\n");
		exit(-1);
	}
	printf("%p\n", p);
	(*p) = 0;
	printf("%d\n", *p);
	//Did we forget something?
	return 0;
}
