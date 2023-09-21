#include <stdio.h>
int main()
{
	int a[10], *p = a, i;

	for(i=0; i<10; i++) *p++ = i;
	
	for(i=0; i<10; i++)
		printf("%d ", i);
	printf("\n");

	//What would happen when we run the following code?
        for(i=0; i<10; i++) *++p = i;

        for(i=0; i<10; i++)
                printf("%d ", i);
        printf("\n");
	
	return 0;
}
