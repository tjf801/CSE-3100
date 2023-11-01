//In this example, we use large number of threads to simulate random walkers
//We calculate the fraction of random walkers that returns to the starting point
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#define MAX 100000
#define NUM_THREADS  30000

struct thread_data
{
	int thread_num;
	int result;
};
//A thread to do a random walk
//A walker starts at x = 0 and randomly go to left or right with equal probability
//Check whether the walk returns to x = 0 within MAX steps 
void* random_walk(void* threadarg)
{

    	struct thread_data* my_data = (struct thread_data*) threadarg;
    
	int x = 0;
    	int count = 0;
	int dir;
    	do
    	{
        	dir = rand() % 2;
		if(dir == 0) x++;
        	else x--;
        	count ++;
    	}while(x !=0  && count < MAX);
    	if(x==0) my_data->result = 1;
    	else my_data->result = 0;
    	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	pthread_t threads[NUM_THREADS];
	struct thread_data thread_data_array[NUM_THREADS];
	int rc, t;

	srand(12345);
    	for( t=0; t<NUM_THREADS; t++ ) {
        	thread_data_array[t].thread_num = t;
        	//printf("Creating thread #%d\n", t);
        	rc = pthread_create(&threads[t], NULL, random_walk, &thread_data_array[t]);

        	if (rc) {
            		printf("ERROR; return code from pthread_create() is %d\n", rc);
            		exit(-1);
        	}
    	}

	//sum will hold number of walkers that returns to x = 0
	int sum = 0;
    	for( t=0; t<NUM_THREADS; t++ ) 
    	{
        	//printf("Joining thread #%d\n", t);
        	rc = pthread_join( threads[t], NULL );
        	if( rc ){
            		printf("ERROR; return code from pthread_join() is %d\n", rc);
            		exit(-1);
        	}
		sum += thread_data_array[t].result; 
    	}
	//return the fraction of walkers that returns to x = 0
	printf("Return prob. = %lf\n", (double)sum/NUM_THREADS);
    	return 0;
}
