//In this example, we practice read-write locks and barriers.
//We simulate a scenario that n game players each individulally plays a game.
//Afterwards, each player saves his or her id and score into an array scores[], 
//and waits for everyone else to finish the game.
//Once everyone is done with game playing and saving the scores, one player 
//checks the array scores[] to see who is the winner and prints out the winner.
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

	pthread_rwlock_wrlock(&rwlock);
	scores[count].id = my_id;
	scores[count].score = score;	
	count ++;
	pthread_rwlock_unlock(&rwlock);

	//wait for all threads to get here
    int rv = pthread_barrier_wait(&barrier);

	if(rv != 0)
	{
		pthread_rwlock_rdlock(&rwlock);
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
		printf("Out of %d players, the winner is %d, with score %d.\n", count, winner, max_score);
		pthread_rwlock_unlock(&rwlock);
	}
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
