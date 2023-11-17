#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

struct thread_data
{
    int    id;
};

pthread_barrier_t barrier;

void* thread_func(void* threadarg)
{
    struct thread_data* my_data = (struct thread_data*) threadarg;
    int my_id = my_data->id;

    printf("Entering thread %d\n", my_id);
    //Look at the print out to see the effect of the barrier
    //We should see two Entering thread lines before the printouts of val values
    pthread_barrier_wait(&barrier);
    int i;
    for(i = 0 ; i < 5; i++)
    {
        printf("val is %d in thread %d \n", i, my_id);
        //to get print outputs from two threads mixed together
        //sleep(0);
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t thread_1, thread_2;
    pthread_barrier_init(&barrier, NULL, 2);

    struct thread_data thread_data_array[2];

    thread_data_array[0].id = 0;
    pthread_create(&thread_1, NULL, (void*)thread_func, &thread_data_array[0]);
    printf("Thread %d created\n", thread_data_array[0].id);

    //make sure the second thread is created very late
    usleep(500);

    thread_data_array[1].id = 1;
    pthread_create(&thread_2, NULL, (void*)thread_func, &thread_data_array[1]);
    printf("Thread %d created\n", thread_data_array[1].id);

    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);

    pthread_barrier_destroy(&barrier);

    return 0;
}
