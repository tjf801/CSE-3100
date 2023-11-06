//In this assignment, we use multiple threads to calculate the sum
// 1*1 + 2*2 + 3*3 + 4*4 + ... + n*n
// Note we should know from CSE2500 that this sum is
// n*(n+1)*(2*n+1)/6
// We a n value, we will create 2*n threads to do the calculation 
// Before you change the code, read the code and run the code and see what
// happens. Do we always get the result sum?
// Read the code carefully and make changes to the code.
// Use mutex to ensure we always get the right sum.

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#define MAX 1000


int n;
unsigned long sum = 0;
pthread_mutex_t mutex;

void* calculate(void* threadarg)
{

    //fill in one line of code blew

    if(n > 0)
    {
        sum += n*n;
        //do not delete the following line of code
        //this line helps us to see wrong results
        sleep(0);
        n--;
    }
    //fill in one line of code below
    
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{

    if(argc!=2)
    {
        printf("Usage: %s n (1 - 500)\n", argv[0]);
        return -1;
    }

    n = atoi(argv[1]);
    assert(n >= 1 && n <= MAX/2);

    //save n to k for later use
    int k = n;
    int m = 2*n;

    //fill in one line of code below

    pthread_t threads[MAX];
    int rc, t;    
    for( t=0; t<m; t++ ) {
        //Since we do not need to pass any arguments to the thread, we pass a NULL
        rc = pthread_create(&threads[t], NULL, calculate, NULL);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for( t=0; t<m; t++ ) 
    {
        rc = pthread_join( threads[t], NULL );
        if( rc ){
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
    }
    //fill in one line of code below

    unsigned long correct_sum = k*(k+1)*(2*k+1)/6;
    printf("thread sum : %ld correct sum : %ld\n", sum, correct_sum);
    return 0;
}
