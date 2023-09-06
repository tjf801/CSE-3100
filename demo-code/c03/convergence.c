//An example to show when to use do while loop
//And when to use while loop
#include <stdio.h>

int main()
{
	//We are trying to find a x such that x = 0.5 * x + 1
	//We use the folliwing algorithm to find it
	double x, y = 1.;

	do
	{	x = y;
		y = 0.5 * x + 1.;
		printf("%lf\n", y);
	}while(y - x > 0.00001 || y - x < -0.00001);
	printf("%lf\n", y);

	//Given an integer n, will n go to 1 by performing the following operations?
	//This is an unsolved problem
	int n;
	printf("Input an integer:\n");
	scanf("%d", &n);
	while(n>1)
	{
		if(n % 2 == 0)
			n = n / 2;
		else
			n = 3*n + 1;
		printf("%d\n", n);
	}
	return 0;
}
