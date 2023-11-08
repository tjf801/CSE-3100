//Note do not be surprised if this code does not work

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#define NUM_CONSUMERS 2
#define FLAG 6

#define SET_BIT(v, i)   (v) |= (1 << (i))
#define CHECK_BIT(v, i)   (v) & (1 << (i))

typedef struct {
    int     data;
    int     ready;
    int     checked;
    pthread_mutex_t mutex;
    pthread_cond_t ready_cond;
    pthread_cond_t processed_cond;
} data_t;

typedef struct {
    int     id;
    int     p;
    data_t  *pdata;
} thread_arg_t;

void * consumer(void *t) 
{
    thread_arg_t * arg = t;

    // get parameters
    int my_id = arg->id;
    int p = arg->p;
    data_t *pdata = arg->pdata;

    int done = 0;

    printf("consumer thread %d(p=%d) started\n", my_id, p);

    while (! done) {
        int     d = -1;

        // lines beween lock and unlock try to get data
		pthread_mutex_lock(&pdata->mutex);

        // wait if data is not ready, or it is ready but I already checked
        while (! pdata->ready || CHECK_BIT(pdata->checked, my_id)) {
            printf("consumer thread %d(p=%d) waiting for data from producer...\n", my_id, p);
            pthread_cond_wait(&pdata->ready_cond, &pdata->mutex);
        }

        if (pdata->data < 0) {
            done = 1;
        }
        else { 
            if ((pdata->data % p) == 0) {
                // it's for me. grab it.
                printf("consumer thread %d(p=%d): use data=%d\n", my_id, p, pdata->data);
                d = pdata->data;
                pdata->ready = 0;
                pthread_cond_signal(&pdata->processed_cond);
            }
            else {
                printf("consumer thread %d(p=%d): checked data=%d. Not for me.\n", my_id, p, pdata->data);
                SET_BIT(pdata->checked, my_id);
                // one can check if all consumers have checked.
				if(pdata->checked == FLAG)
                {
                	printf("Everyone checked.\n");
					d = -1;
					//d = pdata->data;
                    pdata->ready = 0;
                    pthread_cond_signal(&pdata->processed_cond);
                }
            }
        }
		pthread_mutex_unlock(&pdata->mutex);
        // add something to process the data
        if (d >= 0) {
            printf("consumer thread %d(p=%d): done with processing data %d\n", my_id, p, d);
        }
    }
    printf("consumer thread %d(p=%d) exiting\n", my_id, p);
    pthread_exit(NULL);
}

void *producer(void *t) 
{
    thread_arg_t * arg = t;

    // get parameters
    data_t *pdata = arg->pdata;

    int done = 0;

    while (! done) {
        printf("Producer: Enter an integer:\n");
        int v;
       
        if (scanf("%d", &v) != 1) {
            v = -1;
        }

        pthread_mutex_lock(&pdata->mutex);
        while (pdata->ready) {
            printf("producer: waiting for %d to be processed.\n", pdata->data);
            pthread_cond_wait(&pdata->processed_cond, &pdata->mutex);
        }
        pdata->data = v;
        pdata->ready = 1;
        pdata->checked = 0;
        printf("producer placed data %d\n", v);
        // if signal, only one consumer will check
		// pthread_cond_signal(&pdata->ready_cond);
		//if we change pthread_cond_broadcast() to pthread_cond_signal()
		//what will happen?
		//pthread_cond_broadcast(&pdata->ready_cond);
		pthread_cond_signal(&pdata->ready_cond);
        pthread_mutex_unlock(&pdata->mutex);
        done = (v < 0);
    }

    printf("producer exiting...\n");
    pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
    static int primes[] = {2, 3, 5, 7, 11, 13, 0};
    int i, rv;

    data_t  data;

    pthread_t threads[NUM_CONSUMERS + 1];
    thread_arg_t thread_args[NUM_CONSUMERS + 1];

    /* For portability, you can explicitly create threads in a joinable state */
    // define attr, initialize it, set the attribute, and destroy at the end
    // pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    /* Initialize mutex and condition variable objects */
    pthread_mutex_init(&data.mutex, NULL);
    pthread_cond_init (&data.ready_cond, NULL);
    pthread_cond_init (&data.processed_cond, NULL);

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
        assert(thread_args[i].p != 0);

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
