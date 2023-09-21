#include <stdio.h>
int main()
{
	int t[10];

	char * p = (char *) t + 8; 	 // 8 is not scaled
	char * q = (char *) (t + 8); // 8 is scaled
	
	printf("%p\n", t);
	printf("%p\n", p);
	printf("%p\n", q);
	printf("%ld\n", q - p);
	return 0;
}
