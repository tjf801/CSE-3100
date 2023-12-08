#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 4
pthread_barrier_t myBarrier;

void* threadMain(void* threadID) {
    pthread_barrier_wait(&myBarrier);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    pthread_barrier_init(&myBarrier, NULL, NUM_THREADS);
    for (t = 0; t < NUM_THREADS; t++) {
        rc = pthread_create(&threads[t], NULL, threadMain, (void*)t);
    }
    pthread_barrier_wait(&myBarrier);
    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }
    pthread_barrier_destroy(&myBarrier);
    return 0;
}