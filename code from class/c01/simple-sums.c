#include <stdio.h>
#include <stdlib.h>
int main()
{
	int i, n = 100;
	int sum;

    sum = 0;
    for(i = 1; i <= n; i++)
    {
        sum += i;
    }
	printf("total = %d\n", sum);

	sum = 0;
	for(i = 1; i <= n; i += 2)
	{
		sum += i;
	}
	printf("odd sum = %d\n", sum);
	return 0;
}