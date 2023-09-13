//An example of array of characters
//First taste of string functions
//This is a checker puzzle.
//We have 3 black checker pieces and 3 white checker pieces arranged as "BBBOWWW"
//All the black pieces are on the left, and all the white pieces are on the right
//And there is one space in the middle indicated by the character 'O'
//The goal is to rearrange the checker pieces according to the checker rule so that 
//in the end we swap the white and black pieces
//The desired final state is "WWWOBBB"
//The legal moves are either move to an adjacent space, or jump over one piece to a space 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void checkers()
{
	char a[] = "WWWOBBB";
	char b[10];
	int k;
	int steps = 0;

	//learn how to use string functions
	while(strcmp(a, "BBBOWWW"))
	{
		printf("%s\n", a);
		printf("0123456\n");

		printf("step : %d\n", steps + 1);
		printf("How to enter a move:\n");
		printf("2m means move the checker at index 2.\n");
		printf("3j means jump the checker at index 3.\n");
		printf("Enter your move : ");
		scanf("%s", b);
		//calculate the index k: a common trick
		//we get the index from b[0]
		k = b[0] - '0';
		//we get the action from b[1]
		//the action can be move or jump
		//handle move
		if(b[1] == 'm')
		{
			if(k >= 1 && a[k-1] == 'O')
			{
				a[k-1] = a[k];
				a[k] = 'O';
				steps ++;
			}
			if(k < 6 && a[k+1] == 'O')
			{
				a[k+1] = a[k];
				a[k] = 'O';
				steps ++;
			}
			//nothing happens if the move is not legit
		}
		//handle jump
		if(b[1] == 'j')
		{
			if(k >= 2 && a[k-2] == 'O')
            {
                a[k-2] = a[k];
                a[k] = 'O';
                steps ++;
            }
            if(k < 5 && a[k+2] == 'O')
            {
                a[k+2] = a[k];
                a[k] = 'O';
                steps ++;
            }
			//nothing happens if the move is not legit
		}
	}
	printf("Game over! %d steps\n", steps);
}
int main()
{
	checkers();
}
