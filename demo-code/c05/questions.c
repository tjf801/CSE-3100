#include <stdio.h>
#include <stdlib.h>

int main()
{
	int	a[] = {1, 2, 3, 4};	
	int	b[4] = {1, 2, 3};	
	
	int i;
	for(i=0; i<4; i++)
	{
		printf("%d %d %d\n", i, a[i], b[i]);
	}

	char	c[] = {'a', 'b', 'c', 'd'};
	char	d[] = "abcd";
	
	printf("%lu %lu\n", sizeof(c), sizeof(d));

	int	e[10];
	
	printf("%p %p\n", &(e[0]), &(e[4]));

	char t[10][20];
 	printf("%p %p\n", &(t[0][0]), &(t[1][1])); 
	return 0;
}
