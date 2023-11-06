#include "counter.h"
#include <stdlib.h>

SCounter* makeCounter()
{
    SCounter* c = (SCounter*)malloc(sizeof(SCounter));
    if (c == NULL)
        return c;
    pthread_mutex_init(&c->_mtx,NULL);
    c->_val = 0;
    return c;
}

// Assume the mutex is unlocked.
// RTM to see what happens if the mutex is locked.
void     freeCounter(SCounter* c)
{
    if (c == NULL)
        return;
    pthread_mutex_destroy(&c->_mtx);
    free(c);
}

int      incCounter(SCounter* c)
{
    pthread_mutex_lock(&c->_mtx);
    int rv = c->_val++;
    pthread_mutex_unlock(&c->_mtx);
    return rv;
}

int      decCounter(SCounter* c)
{
    pthread_mutex_lock(&c->_mtx);
    int rv = c->_val--;
    pthread_mutex_unlock(&c->_mtx);
    return rv;
}

void     resetCounter(SCounter* c)
{
    pthread_mutex_lock(&c->_mtx);
    c->_val = 0;
    pthread_mutex_unlock(&c->_mtx);
}
