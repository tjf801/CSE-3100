//In this code for thought problem, we complie and run the following code 
//that we showed in class.
//When we run the code and enter n, 
//try 1, 10, 100, 1000, 10000, 100000, 1000000
//Pay attention to whether the program is giving your the correct result
//Note the result of sum from 1 to n should be n*(n+1)/2
//You can use this formula to check the result geneated by the code.
//From the above inputs for n, find the minimum n that geneates a wrong result,
//then first comment out the following part of the code from the main function.
/* 
    int i, n, sum;
    sum = 0;
    printf("Enter n:\n");
    scanf("%d", &n);
    i = 1;
    while (i <= n) {
        sum = sum + i;
        i = i + 1;
    }
    printf("Sum from 1 to %d = %d\n", n, sum); 
*/

//And then uncomment and make changes to the lines of code that follows and complie and 
//run the code again, and submit your work. 
//Note there might be errors in this part of the code, you need to fix them.

//The reason the code breaks when n is large is because the int type is represented by 4 bytes.
//The maximum integer that can be represented by int is 2^31 - 1 = 2147483647.
//If our sum is above this number, we will get a wrong result.
//Note you use the following statement to show the size of int
//printf("The size of int is %ld\n", sizeof(int));

/*
  Compute the sum of the integers
  from 1 to n, for a given n    
*/
#include <stdio.h>

int main(void) {
	int i, n, sum;
	sum = 0;
	printf("Enter n:\n");
	scanf("%d", &n);
	i = 1;
	while (i <= n) {
		sum = sum + i;
		i = i + 1;
	}
	printf("Sum from 1 to %d = %d\n", n, sum);

//	int N = 1000;
//	print("Among 1, 10, 100, 1000, 10000, 100000, 1000000\n");
//	print("The smallest number to break the code is %d\n", N);
	return 0;
}

