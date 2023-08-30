#include <stdio.h>
int main()
{
	unsigned int x = 3;
	unsigned int y = 7;
	unsigned int z = x - y;
	printf("%u\n", z);
	printf("%d\n", z);

	_Bool b = 256;
	printf("%d\n", b);
	char c = 256;
	printf("%d\n", c);
	return 0;
}
