#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#define NUM_CONSUMERS 11

typedef struct {
    int     n;                              //number of data items remaining to be produced
    int     data;			                //data 
    int     ready;			                //ready
    pthread_mutex_t mutex;		            //mutex
    pthread_cond_t cond[NUM_CONSUMERS + 1];	//condition varible array

} data_t;

typedef struct {
    int     id;					//id of the thread
    int     *p_count;			//pointer to count of the dice outcome 
    data_t  *pdata;				//pointer to the data of the type data_t
} thread_arg_t;

//producer generate dice toss results
void *producer(void *t) 
{
    thread_arg_t * arg = t;

    // get parameters
    int id = arg->id;
    data_t *pdata = arg->pdata;
    unsigned s = id; 
    int done = 0;

    while(!done)
    {
        int v1 = rand_r(&s) % 6 + 1;
        int v2 = rand_r(&s) % 6 + 1;
        int v = v1 + v2;
        //Fill in code below


    }
    pthread_exit(NULL);
}

//consumer thread count the dice outcome according to its assignment
void * consumer(void *t) 
{
    thread_arg_t * arg = t;

    // get parameters
    int id = arg->id;
    int *p_count = arg->p_count;
    data_t *pdata = arg->pdata;

    int done = 0;
    while (! done) 
	{
        //TODO:
		//Fill in code below


    }
    pthread_exit(NULL);
}


//Do not change the main() function
int main (int argc, char *argv[])
{

    if(argc != 2)
    {
        printf("Usage: %s n\n", argv[0]);
        exit(0);
    }
    int n = atoi(argv[1]);

    int i, rv;
    int count[NUM_CONSUMERS + 1];

    data_t  data;

    pthread_t threads[NUM_CONSUMERS + 1];
    thread_arg_t thread_args[NUM_CONSUMERS + 1];

    /* Initialize mutex, condition variable objects and count array*/
    pthread_mutex_init(&data.mutex, NULL);
    for(i = 0; i < NUM_CONSUMERS + 1; i++)
    {
        pthread_cond_init(&data.cond[i], NULL);
        count[i] = 0;
    }

    //no data is ready
    data.ready = 0;
    data.n = n;

    // create producer
    thread_args[0].id = 0;
    thread_args[0].pdata = &data;
    rv = pthread_create(&threads[0], NULL, producer, &thread_args[0]);
    assert(rv == 0);


    // create consumers
    for (i = 1; i < NUM_CONSUMERS + 1; i++) {
        // prepare arguments
        thread_args[i].id = i;
        thread_args[i].p_count = &count[i]; 
        thread_args[i].pdata = &data;

        rv = pthread_create(&threads[i], NULL, consumer, &thread_args[i]);
        assert(rv == 0);
    }

    /* Wait for all threads to complete */
    for (i = 0; i < NUM_CONSUMERS + 1; i++) {
	    pthread_join(threads[i], NULL);
    }

    /* Clean up */
    pthread_mutex_destroy(&data.mutex);
    for(i = 0; i < NUM_CONSUMERS + 1; i++)
        pthread_cond_destroy(&data.cond[i]);

    /* print out results */
    for(int i = 1; i < NUM_CONSUMERS + 1; i++)
    {
        printf("%2d: %d\n", i + 1, count[i]);
    }
    return 0;
} 
