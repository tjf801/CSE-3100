#include <stdio.h>
#include <stdlib.h>

int main()
{
	const	int	a = 10;	    // cannot change a
	int b = 20;

	// a pointer to a constant int
	const int  *pa = &a;	 // can change pa, but not *pa
	printf("%d\n", *pa);
	pa = &b;
	printf("%d\n", *pa);
	b = 30;
        printf("%d\n", *pa);
	//the following line of code will not work
	//*pa = b;
	return 0;
}
