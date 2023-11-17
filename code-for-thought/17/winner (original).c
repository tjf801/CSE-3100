//In this assignment, we pratice read-write locks and barriers.
//We simulate a scenario that n game players each individulally plays a game.
//Afterwards, each player saves his or her id and score into an array scores[], //and waits for everyone else to finish the game.
//Once everyone is done with game playing and saving the scores, each player 
//checks the array scores[] to see whether he or she is the winner.
//If the player is the winner, the player will claim to be the winner by print
//out a message.
//For simplicity, we assume there is only one winner.
  
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#define NUM 10000
int count = 0;
struct player_data
{
	int id;
	int score;
};
struct player_data scores[NUM];

pthread_barrier_t barrier;
pthread_rwlock_t rwlock;
struct thread_data
{
    int    id;
};

//a made up game
int fun_game(int n)
{
	int cnt = 0;
	while(n > 1 && n < 100000)
	{
		if(n % 2 == 0) n /= 2;
		else n = 3*n + 1;
		cnt ++;
	}
	return cnt;
}

void* player_func(void* threadarg)
{
    struct thread_data* my_data = (struct thread_data*) threadarg;
    int my_id = my_data->id;

	int score = fun_game(my_id);

	//add one line of code below
	
	scores[count].id = my_id;
	scores[count].score = score;	
	count ++;
	//add one line of code below


	//wait for all threads to get here
	//add one line of code below

	
	//add one line of code below
	
	int max_score = scores[0].score;
	int winner = scores[0].id;
	for(int i=0; i< count; i++)
	{
		if(scores[i].score > max_score)
		{
			max_score = scores[i].score;
			winner = scores[i].id;
		}
	}
	int num = count;
	//add one line of code below
	
	if(winner == my_id)
		printf("Out of %d players, the winner is %d, with score %d.\n", num, my_id, score);
    pthread_exit(NULL);
}

//Do not change the code in main() function
int main(int argc, char *argv[])
{

    if(argc != 2)
    {
        printf("Usage: %s n(100 - 10000)\n", argv[0]);
        return -1;
    }
    int n = atoi(argv[1]);
    assert(n>=100 && n<=10000);
    pthread_t threads[NUM];
    pthread_barrier_init(&barrier, NULL, n);

    struct thread_data thread_data_array[NUM];

    pthread_rwlock_t rwlock;
    pthread_rwlock_init(&rwlock, NULL);

    for(int i=0; i<n; i++)
    {
		//make sure id starts with 1
        thread_data_array[i].id = i + 1;
        pthread_create(&threads[i], NULL, (void*)player_func, &thread_data_array[i]);
    }

    for(int i = 0; i<n; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&barrier);
    pthread_rwlock_destroy(&rwlock);

    return 0;
}
