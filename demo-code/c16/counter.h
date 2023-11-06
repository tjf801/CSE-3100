#ifndef __COUNTER_H_
#define __COUNTER_H_

#include <pthread.h>

typedef struct SCounterTag {
    pthread_mutex_t  _mtx;
    int              _val;
} SCounter;

SCounter* makeCounter();
void     freeCounter(SCounter*);
int      incCounter(SCounter*);
int      decCounter(SCounter*);
void     resetCounter(SCounter*);
#endif
