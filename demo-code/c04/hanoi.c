//Tower of Hanoi
//https://en.wikipedia.org/wiki/Tower_of_Hanoi

#include <stdio.h>
//move n disks from rod a to rod c without ever put larger disks on top of smaller disks
//return the number of moves needed to achieve the goal
int move(int n, char a, char b, char c)
{
	// There is only one disk at a
	// Easy! Just move it to c
	if(n==1)
	{
		printf("Move %d from %c to %c\n", n, a, c);
		return 1;
	}
	// Move the top n - 1 disks from a to b
	int n1 = move(n - 1, a, c, b);
	// Move the largest disk from a to c
	printf("Move %d from %c to %c\n", n, a, c);
	// Move the n - 1 disks from b to c
	int n2 = move(n - 1, b, a, c); 
	return n1 + n2 + 1;
}

int main()
{
	int n;
	
	printf("Enter n: ");
	scanf("%d", &n);
	printf("%d moves.\n", move(n, 'a', 'b', 'c'));
	return 0;
}
//Can you prove that it takes 2^n - 1 moves to move n disks from a to c?

