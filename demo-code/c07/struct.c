#include <stdio.h>
struct struct_random {
    char    x[5];	// bytes 0-4
    int     y;	// bytes 8-11
    double  z;	// bytes 16-23
    char    c;	// byte 24
}a;

struct struct_sorted {
    double  z;	// bytes 0 - 7
    int     y;	// bytes 8 - 11
    char    x[5];	// bytes 12 - 16
    char    c;	// byte  17
}b;

int main()
{
	printf("size of a: %lu\n", sizeof(a));
	printf("size of b: %lu\n", sizeof(b));
	return 0;
}
