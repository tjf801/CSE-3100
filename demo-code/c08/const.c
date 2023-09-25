//try to compile this file and read the message carefully
#include <stdio.h>
#include <stdlib.h>

int main()
{
	// constant int
	const	int	a = 10;	    // cannot change a
	int b = 20;

	a = 20;

	// a pointer to a constant int
	const int  *pa = &a;	 // can change pa, but not *pa
	*pa = b;

	// a constant pointer to an int
	int  * const pb = &b;	 // can change *pb, but not pb
	pb = pa;
	printf("pb = %p\n", pb);

	// a constant pointer to a constant int
	const int  * const pc = &a; // cannot change *pc or pc
	pc = &b;
	*pc = b;
	return 0;
}
