//In this assignment, we use multiple threads to solve a puzzle.
//The puzzle is specified by the following array
//int a[] = {3, 6, 4, 1, 3, 4, 2, 5, 3, 0};
//A walker walks along the index of this array.
//He starts at the index 0, where a[0] is 3, which indicates that
//the walker can move 3 steps. If the walker move to the left, he would be at index
//-3, which is out of the range. Hence, he can only move to the right, after he makes the move,
//he will be at index 3. Since a[3] = 1, he can move to the left or right by one move.

//Note whenever he makes his move, he should be in the range [0, 9]. He is not allowd to move 
//outside of this range.

//The goal of the puzzle is to for the walker to reach index 9, where a[9] = 0. And he will
//stop there.

//What we need to do is to find the solutions of this puzzle. We limit the solutions to have
//at most 10 moves.

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NUM_THREADS  1024

struct thread_data {
    int thread_num;	//(0 to 1023) We use every bit of this number to determine the direction of next moves
    int n;			//maximum number of allowed moves (<=10)
    int moves;		//the number of moves (<=10)
    int b[10];		//array to record all the moves (<=10 moves)
};

//array to describe the puzzle
int a[] = {3, 6, 4, 1, 3, 4, 2, 5, 3, 0};

void print_solution(int b[], int moves) {
    for (int k = 0; k < moves; k++)
        printf("->%d ", b[k]);
    printf("\n");
}

void* SolvePuzzle(void* threadarg) {
    struct thread_data* my_data = (struct thread_data*) threadarg;
    int cur = 0;
    int n = my_data->n;
    my_data->moves = 0;
    
	//We will use every single bit of my_data->thread_num to determine which 
	//direction to go for the next move
	//make a copy of my_data->thread_num to k
    int k = my_data->thread_num;
    for (int i = 0; i < n; i++) {
        if (a[cur] == 0) {
            my_data->b[my_data->moves - 1] = cur;
            pthread_exit(NULL);
        }
        
		//get the last bit of k and use it as the direction for the next move
        int dir = k & 1;
		//shift k to the right by one bit
        k = k >> 1;
        if (dir == 0) {
            if (cur + a[cur] >= 0 && cur + a[cur] < n) {
                cur += a[cur];
                my_data->b[my_data->moves++] = cur;
            }
            else {
				//set the number of moves to -1
                my_data->moves = -1;
                pthread_exit(NULL);
            }
        }
        else {
            if(cur - a[cur] >= 0 && cur - a[cur] < n) {
                cur -= a[cur];
                my_data->b[my_data->moves++] = cur;
            }
            else {
                my_data->moves = -1;
                pthread_exit(NULL);
            }
        }
    }
    
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t threads[NUM_THREADS];
    struct thread_data thread_data_array[NUM_THREADS];
    int rc, t;
    int n = 10;
    
    for (t = 0; t < NUM_THREADS; t++) {
        thread_data_array[t].thread_num = t;
        thread_data_array[t].n = n;
        //printf("Creating thread #%d\n", t);
        rc = pthread_create(&threads[t], NULL, SolvePuzzle, &thread_data_array[t]);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    
    int min_moves = n;
    int min_index = 0;
    for (t = 0; t < NUM_THREADS; t++) {
        //printf("Joining thread #%d\n", t);
        rc = pthread_join(threads[t], NULL );
        if (rc) {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
        if (thread_data_array[t].moves >= 0 && thread_data_array[t].moves < min_moves) {
            min_moves = thread_data_array[t].moves;
            min_index = t;
        }
    }
    
    printf("min_moves = %d min_index = %d\n", min_moves, min_index);
	printf("Shortest solution:\n");
    print_solution(thread_data_array[min_index].b, thread_data_array[min_index].moves);
	printf("Can you see that the number %d encodes all the moves in the shortest solution?\n", min_index);
	printf("Conver %d to binary and you will see why.\n", min_index);
    
    return 0;
}
