#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long count = 0;
pthread_mutex_t  mtx;

void* increase(void *arg) 
{
    long i, inc = *(long *)arg;

    for (i=0; i<inc; i++)
    {	
	pthread_mutex_lock(&mtx);	
        count++;
	pthread_mutex_unlock(&mtx);
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) 
{
    pthread_mutex_init(&mtx,NULL);
    pthread_t tid1, tid2;

    long inc = atol(argc >= 2 ? argv[1] : "100");

    pthread_create(&tid1, NULL, increase, &inc);
    pthread_create(&tid2, NULL, increase, &inc);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    printf("counter is %ld\n", count);
    pthread_mutex_destroy(&mtx);
    return 0;
}
