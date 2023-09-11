// This is a two-player game.
// The first player picks a number between 1 and 10 to start the game. 
// After that, each player aleternatively picks a number between 1 and 10 to add to the opponent's number. The result is his number.
// The player who gets to 100 is the winner.
// This code shows there is a winnings strategy for the first player.

// Can you figure out what is the stategy and why it works?

#include <stdio.h>
#include <stdlib.h>


//user_increment is the opponent's increase in the current round
int my_increment(int user_increment)
{
	int inc;

	inc = 11 - user_increment;
	return inc;
}

int main()
{
	//the first player starts with 1 as a part of the winning strategy
	int cur_number = 1;
	int inc;
	int winnder = 0;
	// think why we are using do-while loop instead of while loop
	do
	{
		printf("Current number is %d. Enter your increment(between 1 and 10).\n", cur_number);
		scanf("%d", &inc);
		if(inc >=1 && inc <=10)
		{
			// user wins (this should not happen)
			if(cur_number + inc == 100)
			{
				winnder = 2;
				cur_number += inc;
			}
			// this should not happen
			else if(cur_number + inc > 100)
				cur_number += inc;
			else
			{
				printf("Current number is %d\n", cur_number + inc);
				int my_inc = my_increment(inc);
				printf("My increment is %d\n", my_inc);
				cur_number += my_inc + inc;
				//computer wins (this always happens)
				if(cur_number == 100)
				{
					winnder = 1;
				}
			}
		}
	}while(cur_number < 100);
	printf("Current number is %d\n", cur_number);
	if(winnder == 0)
		printf("Noboy wins.\n");
	else if(winnder == 1)
		printf("I win.\n");
	else
		printf("You win.\n");
	return 0;
}
