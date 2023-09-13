#include <stdio.h>
#include <stdlib.h>

//note calling this function will change the conent of a[]
void reverse(int a[], int n)
{
	int i;
	for(i=0; i<n/2; i++)
	{
		int temp;
		temp = a[i];
		a[i] = a[n-1-i];
		a[n-1-i] = temp;
	}
}

int main()
{
	int a[100];
	int i;

	int n;
	printf("Enter n : ");
	scanf("%d", &n);
	//What happens when you enter n = 101, and 200?
	for(i=0; i<n; i++)
		a[i] = i;
	reverse(a, n);
	//one should notice the content of a[] changed after the function call
	for(i=0; i<n; i++)
	{
		printf("%d ", a[i]);
	}
	printf("\n");
}
