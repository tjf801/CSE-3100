#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NUM_THREADS  8

struct thread_data
{
    int	  thread_num;
    char* message;
    int   len;          // results
};

void* PrintHello(void* threadarg)
{
    struct thread_data* my_data = (struct thread_data*) threadarg;

    sleep(1 + 5*(my_data->thread_num % 2) );

    my_data->len = strlen(my_data->message);
    printf("Thread #%d: %s  length=%d\n", my_data->thread_num, my_data->message, 
            my_data->len);

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) 
{
    pthread_t threads[NUM_THREADS];
    struct thread_data thread_data_array[NUM_THREADS];
    char* messages[NUM_THREADS];
    int rc, t;

    messages[0] = "English: Hello World!";
    messages[1] = "French: Bonjour, le monde!";
    messages[2] = "Spanish: Hola al mundo";
    messages[3] = "Klingon: Nuq neH!";
    messages[4] = "German: Guten Tag, Welt!";
    messages[5] = "Russian: Zdravstvuyte, mir!";
    messages[6] = "Japan: Sekai e konnichiwa!";
    messages[7] = "Latin: Orbis, te saluto!";


    for( t=0; t<NUM_THREADS; t++ ) {
        thread_data_array[t].thread_num = t;
        thread_data_array[t].message = messages[t];
        printf("Creating thread #%d\n", t);
        rc = pthread_create(&threads[t], NULL, PrintHello, &thread_data_array[t]);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    int grand_total = 0;
    for( t=0; t<NUM_THREADS; t++ ) {
        printf("Joining thread #%d\n", t);
        rc = pthread_join( threads[t], NULL );
        if( rc ){
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
        grand_total += thread_data_array[t].len;
    }
    printf("Grand total = %d\n", grand_total);
    pthread_exit(NULL);
}
