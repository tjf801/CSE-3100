#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#define MAX 20000

struct thread_data
{
    int thread_num;
    int num_tickets;
};

int n;	//number of available tickets

void* ticket_agent(void* threadarg)
{

    struct thread_data* my_data = (struct thread_data*) threadarg;   
    if(n > 0)
    {
        my_data->num_tickets = 1;
        printf("Number of tickets : %d by %d\n", my_data->num_tickets, my_data->thread_num);
        n --; 
    }
    else
    {
        my_data->num_tickets = 0;
        printf("Number of tickets : %d by %d\n", my_data->num_tickets, my_data->thread_num);        
    }
    pthread_exit(NULL);
}
int main(int argc, char *argv[])
{

    if(argc!=2)
    {
        printf("Usage: %s n\n", argv[0]);
        return -1;
    }

    n = atoi(argv[1]);
    assert(n >= 1 && n <= MAX/2);

    //m is the number of agents that sell tickets
    //m is two times the number of available tickets
    int m = 2*n;
    pthread_t threads[MAX];
    struct thread_data thread_data_array[MAX];
    int rc, t;    
    for( t=0; t<m; t++ ) {
        thread_data_array[t].thread_num = t;
        //printf("Creating thread #%d\n", t);
        rc = pthread_create(&threads[t], NULL, ticket_agent, &thread_data_array[t]);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    int total = 0;
    for( t=0; t<m; t++ ) 
    {
        //printf("Joining thread #%d\n", t);
        rc = pthread_join( threads[t], NULL );
        if( rc ){
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
        total += thread_data_array[t].num_tickets;
    }
    printf("Total number of tickets sold: %d\n", total);
    return 0;
}
