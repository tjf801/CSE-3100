#include <stdio.h>
int main()
{
	int i, n=1000000000;
	double sum1, sum2;

	printf("sum1:\n");
	sum1 = 0.;
	for(i=1; i<=n; i++)
	{
		sum1 += 1./i;
		if(i % 100000000 == 0)
		{
			printf("%d, %lf\n", i, sum1);
		}
	}
	//printf("sum1 = %lf\n", sum1);

	printf("sum2:\n");
	sum2 = 0.;
	for(i=1; i<=n; i++)
	{
		sum2 += 1./i/i;
		if(i % 100000000 == 0)
		{
			printf("%d, %lf\n", i, sum2);
		}
	}

	//printf("sum2 = %lf\n", sum2);

	printf("PI*PI/6=%lf\n", 3.14927*3.14927/6.);
	return 0;
}