//In this example we use multiple threads to calculate the value of PI
//We use the Bailey–Borwein–Plouffe formula for this purpose
//https://en.wikipedia.org/wiki/Bailey%E2%80%93Borwein%E2%80%93Plouffe_formula
//The formula is the following.
//PI = \sum_{n=0}^\infty \frac{1}{16^n} (\frac{4}{8n+1} - \frac{2}{8n+4} - \frac{1}{8n+5} - \frac{1}{8n+6})
//We use a global array pi[] to hold 4/(8n+1) - 2/(8n+4) - 1/(8n+5) - 1/(8n+6) for each n. 
//That is pi[n] = 4/(8n+1) - 2/(8n+4) - 1/(8n+5) - 1/(8n+6)
//In the main() function, 20 threads will be launched. Each is passed with a parameter n.
//Each of these threads will calculate 4/(8n+1) - 2/(8n+4) - 1/(8n+5) - 1/(8n+6) and save it to pi[n].
//When all the threads are done, in the main() fucntion, PI is calculated based on the formula.
//That is PI = \sum_{n=0}^{19} \frac{1}{16^n} pi[n]
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NUM_THREADS  20

long double pi[NUM_THREADS];

struct thread_data
{
    int thread_num;
};

//A thread to calcuate one term in the PI calculation
void* hex_pi(void* threadarg)
{

	struct thread_data* my_data = (struct thread_data*) threadarg;
	int n = my_data->thread_num;    

	//note how the thread writes to the array pi[]
	pi[n] = (long double)4/(8*n + 1) - (long double)2/(8*n + 4) - (long double)1/(8*n + 5) - (long double)1/(8*n + 6);    
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
   
	pthread_t threads[NUM_THREADS];
	struct thread_data thread_data_array[NUM_THREADS];
	int rc, t;

	for( t=0; t<NUM_THREADS; t++ ) {
        	thread_data_array[t].thread_num = t;
        	rc = pthread_create(&threads[t], NULL, hex_pi, &thread_data_array[t]);
        	if (rc) {
            		printf("ERROR; return code from pthread_create() is %d\n", rc);
            		exit(-1);
        	}
    	}

	long double sum = 0.;
	long double scale = 1.;
    	for( t=0; t<NUM_THREADS; t++ ) 
    	{
        	rc = pthread_join( threads[t], NULL );
        	if( rc ){
            		printf("ERROR; return code from pthread_join() is %d\n", rc);
            		exit(-1);
        	}
		sum += pi[t]*scale;
		scale /= (long double)16.;
    	}
	printf("PI = %.18Lf\n", sum);
    	return 0;
}
