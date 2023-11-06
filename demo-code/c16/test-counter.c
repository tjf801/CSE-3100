#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

#include "counter.h"

SCounter *x;

void * increase(void * p)
{
    long i;

    long cnt = * (long *)p;

    for (i = 0; i < cnt; i++) {
        incCounter(x);
    }
    return NULL;
}

int main(int argc,char* argv[])
{
    pthread_t tid1, tid2;

    int status;

    if (argc < 2) {
        printf("Usage: %s n\n", argv[0]);
        exit(-1);
    }

    long cnt = atol(argv[1]);

    x = makeCounter();
    assert(x != NULL);

    // x does not have to be global.
    // one can define a structure to pass both x and cnt to threads
    status = pthread_create(&tid1, NULL, increase, &cnt);
    assert(status == 0);
    status = pthread_create(&tid2, NULL, increase, &cnt);
    assert(status == 0);

    // we do not care about retval for now.
    status = pthread_join(tid1, NULL);
    status = pthread_join(tid2, NULL);

    // could add a function to read the value. 
    printf("counter is %d\n", x->_val);

    freeCounter(x);
    return 0;
}
