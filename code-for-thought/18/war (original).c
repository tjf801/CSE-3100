//In this assignment, we practice barriers. 
//This is an example where we reuse a barrier.
//We simulate two players playing a card game. The game is similar to the card 
//game War. The game is simplified to facilitate the implementation.
//In each round, both players get a card out and compare whose card is bigger.
//We only compare the rank, where A is the largest and 2 is the smallest.
//The winner takes both cards.
//If it is a tie, then both cards are discarded.
//The game continues until one player loses all the cards or both players have
//played MAX number of rounds.

//Note we reused a barrier and we did not use any mutex.
  
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#define NUM 2
#define NUM_CARDS 52
#define MAX 10000

pthread_barrier_t barrier;
struct thread_data
{
    int id;
	int hand[NUM_CARDS];	//this will be managed as a stack
	int no_cards;
};

int deck[NUM];
char str_cards[NUM_CARDS][NUM + 1];

void card_symbol(int card, char *symbol)
{
	assert(card >=0 && card < NUM_CARDS);
	int suit = card % 4;
	switch(suit)
	{
		case 0: symbol[0] = 'c'; break;
		case 1: symbol[0] = 'd'; break;
		case 2: symbol[0] = 'h'; break;
		case 3: symbol[0] = 's'; break;
	}

	int rank = card / 4;
	if(rank < 8) symbol[1] = '2' + rank;
	else
	{
		switch(rank)
		{
			case 8:	symbol[1] = '0'; break;
			case 9: symbol[1] = 'J'; break;
			case 10: symbol[1] = 'Q'; break;
			case 11: symbol[1] = 'K'; break;
			case 12: symbol[1] = 'A'; break;
		}
	}
	symbol[2] = '\0';
}

//A nice way to shuffle cards
void shuffle(int cards[NUM_CARDS])
{
	for(int i = NUM_CARDS -1; i>=0; i--)
	{
		int k = rand() % (i+1);
		int temp = cards[i];
		cards[i] = cards[k];
		cards[k] = temp; 
	}
}

void* player_func(void* threadarg)
{
    struct thread_data* my_data = (struct thread_data*) threadarg;
    int id = my_data->id;
	int *hand = my_data->hand;
	int no_cards = my_data->no_cards;
	int rounds = 0;
	int tie_count = 0;

	while(no_cards > 0 && no_cards + 2*tie_count < NUM_CARDS && rounds < MAX)
	{
	
		int my_card = hand[no_cards - 1];
		//each player makes change at its own deck location
		deck[id - 1] = my_card;
		//fill in two lines of code



		//everyone is here
		//check results
		int win = 0;
		int lose = 0;
		int tie = 0;

		for(int i=0; i<NUM; i++)
		{
			if(i != id -1)
			{

				if(my_card /4 > deck[i] /4)
					win = 1;
				else if(my_card /4 < deck[i] /4)
					lose = 1;
				else
					tie = 1;	
			}	
		}
		rounds ++;
    	int rv = pthread_barrier_wait(&barrier);
		if(rv != 0)
		{
			printf("-----------------------------------------------------------\n");
		}
		//everyone has checked
		if(win)
		{
	    	for(int i=0; i<NUM; i++)
    		{
        		if(deck[i] >= 0)
        		{
            		hand[no_cards++] = deck[i];
            		deck[i] = -1;
        		}
    		}
			printf("Round %05d: player %d wins with card  %s. %2d cards in hand.\n", rounds, id, str_cards[my_card], no_cards);
		}
		if(lose)
		{
			printf("Round %05d: player %d loses with card %s. %2d cards in hand.\n", rounds, id, str_cards[my_card], no_cards);
		}
		if(tie)
		{
			tie_count ++;
			printf("Round %05d: player %d ties with card  %s. %2d cards in hand.\n", rounds, id, str_cards[my_card], no_cards);
		}
		//fill in one line of code
	
	}
	if(rounds == MAX)
		printf("It is a tie.\n");
	else if(no_cards > 0)
		printf("The winner is player %d.\n", id);
    pthread_exit(NULL);
}

//Do not change the code in main() function
int main(int argc, char *argv[])
{

    if(argc != 2)
    {
        printf("Usage: %s seed\n", argv[0]);
        return -1;
    }

	for(int card = 0; card < NUM_CARDS; card++)
	{
		card_symbol(card, str_cards[card]);
		//printf("%d %s\n", card, str_cards[card]);
	}
	
    int seed = atoi(argv[1]);
	srand(seed);
	int cards[NUM_CARDS];
	for(int i=0; i < NUM_CARDS; i++)
	{
		cards[i] = i;
	}
	shuffle(cards);
	for(int i=0; i< NUM; i++)
	{
		deck[i] = -1;
	}

    int n = 2;
    pthread_t threads[NUM];
    pthread_barrier_init(&barrier, NULL, n);

    struct thread_data thread_data_array[NUM];


    for(int i=0; i<n; i++)
    {
		//make sure id starts with 1
        thread_data_array[i].id = i + 1;
		for(int k = 0; k < NUM_CARDS/NUM; k++)
			thread_data_array[i].hand[k] = cards[k*NUM + i];
		thread_data_array[i].no_cards = NUM_CARDS/NUM;
        pthread_create(&threads[i], NULL, (void*)player_func, &thread_data_array[i]);
    }

    for(int i = 0; i<n; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&barrier);

    return 0;
}
