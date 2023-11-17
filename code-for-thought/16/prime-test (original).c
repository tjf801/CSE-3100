//In this assignment, we use mutex and condition variables to test whether 
//a sequence of integers from the standard input are prime numbers.

//The integers from the standard input are between 100 and 10,000. To test 
//whether an integer in this range is a prime number, we only need to test 
//whether this number can be divided by one of the 25 prime numbers below 100. 
//If the number can be divided by one of these prime numbers, it is not a prime. 
//Otherwise, this number is a prime number. 
//The list of the 25 prime numbers is provided in the code.

//We implement this problem as a producer-consumer problem. A producer thread 
//will read an integer from the standard input and give this number to 25 
//consumer threads to test whether the number is a prime number.

//Each of the 25 threads is assigned a prime number. It will check whether the 
//number in question can be divided by the assigned prime number.

//When a number is ready to be checked, that is, the producer has produced a number, 
//each of the consumer thread will test whether the produced number is a multiple 
//of its prime number. If the number is a multiple of this prime number, the consumer
// thread will indicate that this number is not a prime number, and will ask the 
//producer thread to produce another number.

//Otherwise, if all the threads did the test and none of the test results is positive, 
//the last thread that does the test will declare the number to be a prime number, 
//and notify the producer to produce the next number. 

//Note that, to know whether a consumer thread has already checked the number in 
//question, we use SET_BIT and CHECK_BIT macro provided in the program.

//In this way, a sequence of integers will be tested, until the integer in the 
//standard input is out of the range between 100 and 10,000. The program terminates 
//if the entered number is outside the range (<100 or >10,000).

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#define NUM_CONSUMERS 25

#define FLAG 0x3FFFFFE    //the flag to check whether all 25 consumers have finished CHECK_BIT

#define SET_BIT(v, i)   (v) |= (1 << (i))    //used by consumer i to set the bit on v
#define CHECK_BIT(v, i)   (v) & (1 << (i))    //used by consumer i to check the bit on v

typedef struct {
    int     data;                //integer to be tested
    int     ready;                //data ready for testing?
    int     checked;            //used by SET_BIT and CHECK_BIT 
    pthread_mutex_t mutex;        //mutex
    pthread_cond_t ready_cond;    //ready condition varible
    pthread_cond_t processed_cond;    //processed condition variable
} data_t;

typedef struct {
    int     id;                    //id of the thread
    int     p;                    //prime number for the thread
    data_t  *pdata;                //pointer to the data of the type data_t
} thread_arg_t;

//consumer thread tests if the data can be divided by the prime number owned by the thread
void * consumer(void *t) 
{
    thread_arg_t * arg = t;

    // get parameters
    int my_id = arg->id;
    int p = arg->p;
    data_t *pdata = arg->pdata;

    int done = 0;

    while (! done) 
    {
        //Fill in code below on mutex and condition variable






        //if the data produced is negative, we set done to 1
        if (pdata->data < 0) {
            done = 1;
        }
        else 
        { 
            if ((pdata->data % p) == 0) 
            {
                //fill in the code below



            }
            else 
            {
                //fill in the code below




            }
        }
		//fill in the code below
    }
    return NULL;
}

//read an integer. if it is not in the range it will be set to -1
int read_integer()
{
        printf("Enter an integer: (100 to 10000)\n");
        int v;
        
        if (scanf("%d", &v) != 1) {
            v = -1;
        }   
        
        //make sure v is in the range
        if(v < 100 || v >10000) v = -1;

    return v;
}

void *producer(void *t) 
{
    thread_arg_t * arg = t;

    // get parameters
    data_t *pdata = arg->pdata;

    int done = 0;
    while (! done) 
    {
        //Fill in code below on mutex and condition variable




        //read an integer to v
        int v = read_integer();
        //fill in the code below to produce data and wake up consumers
        
        



        done = (v < 0);
    }

    return NULL;
}

//Do not change the main() function
int main (int argc, char *argv[])
{

    //There are 25 pirmes below 100
    static int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};
    int i, rv;

    data_t  data;

    pthread_t threads[NUM_CONSUMERS + 1];
    thread_arg_t thread_args[NUM_CONSUMERS + 1];

    /* Initialize mutex and condition variable objects */
    pthread_mutex_init(&data.mutex, NULL);
    pthread_cond_init(&data.ready_cond, NULL);
    pthread_cond_init(&data.processed_cond, NULL);

    data.ready = 0; // no data is ready yet

    // create a producer
    thread_args[0].id = 0;
    thread_args[0].p = 0;
    thread_args[0].pdata = &data;
    rv = pthread_create(&threads[0], NULL, producer, &thread_args[0]);
    assert(rv == 0);

    // create consumers
    for (i = 1; i <= NUM_CONSUMERS; i ++) {
        // prepare arguments
        thread_args[i].id = i;
        thread_args[i].p = primes[i-1]; 
        thread_args[i].pdata = &data;

        rv = pthread_create(&threads[i], NULL, consumer, &thread_args[i]);
        assert(rv == 0);
    }

    /* Wait for all threads to complete */
    for (i = 0; i <= NUM_CONSUMERS; i++) {
        pthread_join(threads[i], NULL);
    }

    /* Clean up and exit */
    pthread_mutex_destroy(&data.mutex);
    pthread_cond_destroy(&data.ready_cond);
    pthread_cond_destroy(&data.processed_cond);
    return 0;
} 
