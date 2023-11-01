//In this example, we use multiple threads to calculate a sum that trying to
//solve the Basel problem
//That is, what is the value of \sum _{n=1}^{\infty }{\frac {1}{n^{2}}}
//The answer should be \frac{\pi^2}{6},
//which is approximately 1.644934 
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NUM_THREADS  3
struct thread_data
{
	int thread_num;
	int i, j;
	double sum;          // results
};

void* seq_sum(void* threadarg)
{
	struct thread_data* my_data = (struct thread_data*) threadarg;

	int k;

	printf("i=%d j=%d\n", my_data->i, my_data->j);	
	for(k=my_data->i; k<=my_data->j; k++)
	{
		//printf("k = %d\n", k);
		my_data->sum += 1./k/k;
	}
    	printf("Thread #%d: sum=%lf\n", my_data->thread_num, my_data->sum);

    	pthread_exit(NULL);
}

int main(int argc, char* argv[]) 
{
	if(argc!=2)
	{
		printf("Usage: %s n\n", argv[0]);
		exit(-1);
	}
	int n = atoi(argv[1]);

	pthread_t threads[NUM_THREADS];
	struct thread_data thread_data_array[NUM_THREADS];
	int rc, t;
	for( t=0; t<NUM_THREADS; t++ ) {
        	thread_data_array[t].thread_num = t;
        	thread_data_array[t].i = 1 + t*n/NUM_THREADS;
		thread_data_array[t].j = (t+1)*n/NUM_THREADS;
        	printf("Creating thread #%d\n", t);
        	rc = pthread_create(&threads[t], NULL, seq_sum, &thread_data_array[t]);
        	if (rc) {
            		printf("ERROR; return code from pthread_create() is %d\n", rc);
            		exit(-1);
        	}
    	}
    	double grand_sum = 0.;
    	for( t=0; t<NUM_THREADS; t++ ) {
        	printf("Joining thread #%d\n", t);
        	rc = pthread_join( threads[t], NULL );
        	if( rc ){
            		printf("ERROR; return code from pthread_join() is %d\n", rc);
            		exit(-1);
        	}
        	grand_sum += thread_data_array[t].sum;
    	}
    	printf("Grand sum = %lf\n", grand_sum);
    	return 0;
}
