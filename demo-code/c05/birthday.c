#include <stdio.h>
#include <stdlib.h>
#define M 365

//Do we have at least two people with the same birthday among n people?
int same_birthday(int n)
{
	//Note how the array birthday_count[M] is initilized
	int birthday_count[M] = {0};
	int i;

	for(i=0; i<n; i++)
	{
		//randomly generate a birthday 
		int day = rand() % M;
		//note the following line code. This technique is widely used
		birthday_count[day] ++;
		//early return since we already can answer the question
		if(birthday_count[day] == 2) return 1;
	}
	return 0; 		 
}

int main()
{
	int trials = 1000000;
	int i, n;

	srand(12345);	
	printf("Enter n: ");
	if(scanf("%d", &n)!=1) return -1;
	int count = 0;
	for(i=0; i< trials; i++)
	{
		count += same_birthday(n);
	}
	printf("P(Among %d people, at least two people have the same birthday)=%.5lf\n", n, (double)count/trials);
	return 0;
}
