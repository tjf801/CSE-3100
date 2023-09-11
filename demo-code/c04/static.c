#include <stdio.h>

void seq(int n)
{
	static int odd_count = 0;
	
	printf("n = %d\n", n);
	if(n==1)
	{
		odd_count += 1;
		printf("odd count = %d\n", odd_count);
		return ;
	}
	if(n % 2 ==0)
		seq(n/2);
	else
	{
		odd_count ++;
		printf("odd count = %d\n", odd_count);
		seq(3*n + 1);
	}
}

int main()
{
	int n;
	
	printf("Enter n: ");
	scanf("%d", &n);
	seq(n);
	//What if you run seq(n) again?
	printf("***************\n");
	seq(n);
	//Are you getting what you want?
	return 0;
}
