//In this assignment, we write code to convert decimal integers into hexadecimal numbers
//We pratice using arrays in this assignment
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//convert the decimal integer d to hexadecimal, the result is stored in hex[]
void dec_hex(const int d, char hex[80])
{
	const static char digits[] ={'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B',
					'C', 'D', 'E', 'F'};
	
	int k = 0;
	//Fill in your code below
	//It should not be hard to obtain the last digit of a hex number
	//Then what?
	//If we are getting the digits in a reverse order, what should we do in the end?
	
	const unsigned int num_hex_digits = ({ // this will probably get optimized to a single `bsr` instruction, i hope
		unsigned int floor_log2 = 0;
		k = d;
		while (k >>= 1) ++floor_log2;
		(floor_log2 >> 2) + 1;
	});
	
	for (k = 0; k < num_hex_digits; k++)
		hex[num_hex_digits - k - 1] = digits[(d >> (k << 2)) & 15];
	
	//Make sure the last character is a zero so that we can print the string correctly
	hex[k] = '\0';
}

// Do not change the code below
int main()
{
	int d;
	char hex[80];
	
	printf("Enter a positive integer : ");
	scanf("%d", &d); 
	dec_hex(d, hex);
	printf("%s\n", hex);
	return 0;
}
