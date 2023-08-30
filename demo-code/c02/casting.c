#include <stdio.h>
int main()
{
	int x = 10;
	int y = 3;

	double z = x / y;

	printf("z=%lf\n", z);

	int x1 = 10;
	int y1 = 3;
	double z1 = (double)x1 / y1;
	printf("z1=%lf\n", z1);

    int x2 = 10;
    int y2 = 3;
    double z2 = (double)(x2 / y2);
    printf("z2=%lf\n", z2);
	return 0;
}
