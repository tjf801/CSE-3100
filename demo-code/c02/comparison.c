#include <stdio.h>

int main()
{
	double x = 7e+33;
	double y = 0.001;
	
	//What do you think this output would be?
	printf("%d\n", x < x + y);
	
	//What do you think this output would be?
	printf("%d\n", x == x + y);

	//Now print both numbers
	printf("%lf\n", x);
	printf("%lf\n", x + y);
	
	return 0;
}
