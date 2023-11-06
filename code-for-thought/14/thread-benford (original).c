//In this assignment, we use multiple threads to calculate the frequencies of the first digits
//of the numbers in an array of long integers
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

//#define NUM_THREADS 2
#define DIGITS 10
#define MAX 100000000
unsigned long a[MAX];

struct thread_data
{
    int thread_num;
    int i, j;							//the staring and ending index
    unsigned long freq[DIGITS];			// results
};

//initialize the array
void init_array(unsigned long a[], int n)
{   
    a[0] = 1; 
    for(int i = 1; i < n; i++)
    {   
        a[i] = a[i-1] + i + 1;
    }
}

//get the first digit of the long integer k
int first_digit(unsigned long k)
{
    while(k >= DIGITS)
	{
        k = k/DIGITS;
    }
    return k;
}
//frequencies of the first digits among the numbers in a[],
//there are total n numbers
void frequencies(unsigned long a[], int n, unsigned long freq[DIGITS])
{
    for(int k=0; k<DIGITS; k++)
        freq[k] = 0;

    for(int i = 0; i<n; i++)
    {
        int m = first_digit(a[i]);
        freq[m] ++;
    }
}

void* thread_freq(void* threadarg)
{
    struct thread_data* my_data = (struct thread_data*) threadarg;
	//fill in code below, only line is needed
    
	pthread_exit(NULL);
}

int main(int argc, char* argv[]) 
{
	if(argc!=3)
	{
		printf("Usage: %s n NUM_THREADS\n", argv[0]);
		exit(-1);
	}
	int n = atoi(argv[1]);
	assert(n>=1 && n<=MAX);
	int NUM_THREADS = atoi(argv[2]);
	assert(NUM_THREADS >=1 && NUM_THREADS <=10);

	init_array(a, n);
	
    pthread_t threads[NUM_THREADS];
    struct thread_data thread_data_array[NUM_THREADS];
    int rc, t;
	for( t=0; t<NUM_THREADS; t++ ) {
        thread_data_array[t].thread_num = t;
        thread_data_array[t].i = t*n/NUM_THREADS;
		thread_data_array[t].j = (t+1)*n/NUM_THREADS - 1;
        rc = pthread_create(&threads[t], NULL, thread_freq, &thread_data_array[t]);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    unsigned long freq[DIGITS] ={0};
    for( t=0; t<NUM_THREADS; t++ ) {
        rc = pthread_join( threads[t], NULL );
        if( rc ){
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
		//fill in code below
		//no more than two lines of code expected


    }
    for(int i=1; i<DIGITS; i++)
        printf("%d: %8lu\n", i, freq[i]);
    return 0;
	//Run this code using different number of threads and see whether multiple threads
	//lead to time saving
	//Compare the running times with using just 1 thread
}
