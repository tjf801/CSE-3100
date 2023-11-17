#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#define NUM 100
int count = 0;
pthread_barrier_t barrier;
pthread_rwlock_t rwlock;
struct thread_data
{
    int    id;
};

void* thread_func(void* threadarg)
{
    struct thread_data* my_data = (struct thread_data*) threadarg;
    int my_id = my_data->id;

    pthread_barrier_wait(&barrier);
    if(my_id % 3 == 0 || my_id % 3 == 2)
    { 
        pthread_rwlock_rdlock(&rwlock);
        printf("reading: count = %d\n", count);
        pthread_rwlock_unlock(&rwlock); 
    }
    else
    {
        pthread_rwlock_wrlock(&rwlock);
        count ++;
        printf("writing: count = %d\n", count);
        pthread_rwlock_unlock(&rwlock);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{

    if(argc != 2)
    {
        printf("Usage: %s n(10 - 100)\n", argv[0]);
        return -1;
    }
    int n = atoi(argv[1]);
    assert(n>=10 && n<=100);
    pthread_t threads[NUM];
    pthread_barrier_init(&barrier, NULL, n);

    struct thread_data thread_data_array[NUM];

    pthread_rwlock_t rwlock;
    pthread_rwlock_init(&rwlock, NULL);

    for(int i=0; i<n; i++)
    {
        thread_data_array[i].id = i;
        pthread_create(&threads[i], NULL, (void*)thread_func, &thread_data_array[i]);
        printf("Thread %d created\n", thread_data_array[i].id);
    }

    for(int i = 0; i<n; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&barrier);
    pthread_rwlock_destroy(&rwlock);

    return 0;
}
