#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ABS(N) ((N<0)?(-N):(N))

double two_d_random(const int n)
{

	//Fill in code below
	//When deciding which way to go for the next step, generate a random number as follows.
	//r = rand() % 4;
	//Treat r = 0, 1, 2, 3 as up, right, down and left respectively.
	
	//The random walk should stop once the x coordinate or y coordinate reaches $-n$ or $n$. 
	//The function should return the fraction of the visited $(x, y)$ coordinates inside (not including) the square.
	// i love VLAs <3
	_Bool touched_squares[2*n-1][2*n-1];
	memset(touched_squares, 0, sizeof touched_squares);
	
	int current_x = 0, current_y = 0;
	while (ABS(current_x) < n && ABS(current_y) < n) {
		touched_squares[n+current_x-1][n+current_y-1] = 1;
		switch (rand() % 4) {
			case 0: current_y++; break;
			case 1: current_x++; break;
			case 2: current_y--; break;
			case 3: current_x--; break;
		}
	}
	
	unsigned int sum = 0;
	for (int i = 0; i < 2*n-1; ++i)
		for (int j = 0; j < 2*n-1; ++j) {
			sum += touched_squares[i][j];
		}
	
	return ((double)sum)/((double)(2*n-1)*(2*n-1));
}

//Do not change the code below
// it literally does not compile.
// i HAVE to change main from `main()` to `main(int argc, char **argv)`
// like how tf does your editor not underline the undefined `argc` and `argv` variables????
int main(int argc, char **argv)
{
	int trials = 1000;
	int i, n, seed;
	if (argc == 2) seed = atoi(argv[1]);
	else seed = 12345;

	srand(seed);
	for(n=1; n<=64; n*=2)
	{	
		double sum = 0.;
		for(i=0; i < trials; i++)
		{
			double p = two_d_random(n);
			sum += p;
		}
		printf("%d %.3lf\n", n, sum/trials);
	}
	return 0;
}

