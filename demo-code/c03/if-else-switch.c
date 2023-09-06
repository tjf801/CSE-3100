#include <stdio.h>
int main()
{
	int n = 0x123ABC;

	//We want to print out the last digit of the hex number n
	//Below is the first implementation using if else statements
	
	if(n % 16 == 0)
		printf("The last digit of the hex number %0x is 0.\n", n);
	else if(n % 16 == 1)
        	printf("The last digit of the hex number %0x is 1.\n", n);
	else if(n % 16 == 2)
        	printf("The last digit of the hex number %0x is 2.\n", n);
    	else if(n % 16 == 3)
        	printf("The last digit of the hex number %0x is 3.\n", n);
    	else if(n % 16 == 4)
        	printf("The last digit of the hex number %0x is 4.\n", n);
    	else if(n % 16 == 5)
        	printf("The last digit of the hex number %0x is 5.\n", n);
    	else if(n % 16 == 6)
        	printf("The last digit of the hex number %0x is 6.\n", n);
    	else if(n % 16 == 7)
        	printf("The last digit of the hex number %0x is 7.\n", n);
    	else if(n % 16 == 8)
        	printf("The last digit of the hex number %0x is 8.\n", n);
    	else if(n % 16 == 9)
        	printf("The last digit of the hex number %0x is 9.\n", n);
    	else if(n % 16 == 10)
        	printf("The last digit of the hex number %0x is A.\n", n);
    	else if(n % 16 == 11)
        	printf("The last digit of the hex number %0x is B.\n", n);
    	else if(n % 16 == 12)
        	printf("The last digit of the hex number %0x is C.\n", n);
    	else if(n % 16 == 13)
        	printf("The last digit of the hex number %0x is D.\n", n);
    	else if(n % 16 == 14)
        	printf("The last digit of the hex number %0x is E.\n", n);
    	else
        	printf("The last digit of the hex number %0x is F.\n", n);
   

	//Can you correctly nest the above if else statements?
	//Note in C there is no elif statement as in Python.

	//The above implementation is not nice. I guess you do not like it.
	//Below is the second implementation using a switch statement

	switch(n % 16)
	{
		case 0:
			printf("The last digit of the hex number %0x is 0.\n", n);
			break;
		case 1:
            		printf("The last digit of the hex number %0x is 1.\n", n);
            		break;
		case 2:
            		printf("The last digit of the hex number %0x is 2.\n", n);
            		break;
        	case 3:
            		printf("The last digit of the hex number %0x is 3.\n", n);
            		break;
        	case 4:
            		printf("The last digit of the hex number %0x is 4.\n", n);
            		break;
        	case 5:
            		printf("The last digit of the hex number %0x is 5.\n", n);
            		break;
        	case 6:
            		printf("The last digit of the hex number %0x is 6.\n", n);
            		break;
        	case 7:
            		printf("The last digit of the hex number %0x is 7.\n", n);
            		break;
        	case 8:
            		printf("The last digit of the hex number %0x is 8.\n", n);
            		break;
        	case 9:
            		printf("The last digit of the hex number %0x is 9.\n", n);
            		break;
        	case 10:
            		printf("The last digit of the hex number %0x is A.\n", n);
            		break;
        	case 11:
            		printf("The last digit of the hex number %0x is B.\n", n);
            		break;
        	case 12:
            		printf("The last digit of the hex number %0x is C.\n", n);
            		break;
        	case 13:
            		printf("The last digit of the hex number %0x is D.\n", n);
            		break;
        	case 14:
            		printf("The last digit of the hex number %0x is E.\n", n);
            		break;
        	case 15:
            		printf("The last digit of the hex number %0x is F.\n", n);
            		break;
	}
	//This is better. We can do better than the above.

	//Implementation #3:
	switch(n % 16)
    	{
        	case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
            		printf("The last digit of the hex number %0x is %c.\n", n, '0' + n % 16);
            		break;
		case 10: case 11: case 12: case 13: case 14: case 15:
			printf("The last digit of the hex number %0x is %c.\n", n, 'A' + n % 16 - 10);
            		break;
	}

	//Once we see the above implementation, we know we can do even better.
	//Implementation #4:
	if(n % 16 <=9) 
		printf("The last digit of the hex number %0x is %c.\n", n, '0' + n % 16);
	else
		printf("The last digit of the hex number %0x is %c.\n", n, 'A' + n % 16 - 10);

	//One last implementation using arrary
	char a[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	
	printf("The last digit of the hex number %0x is %c.\n", n, a[n % 16]);

	return 0;
}
