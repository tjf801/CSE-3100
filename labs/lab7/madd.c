#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include "matrix.h"

#define     MAX_THREADS     2
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

typedef struct {
    unsigned int id;
    TMatrix *m, *n, *t;
} thread_arg_t;

/* the main function of threads */
static void *thread_main(void * p_arg) {
    thread_arg_t *thread_args = (thread_arg_t*) p_arg;
    
    // assert(thread_args->m->nrows == thread_args->n->nrows && thread_args->m->nrows == thread_args->t->nrows);
    // assert(thread_args->m->ncols == thread_args->n->ncols && thread_args->m->ncols == thread_args->t->ncols);
    int matrix_rows = thread_args->m->nrows;
    int matrix_cols = thread_args->m->ncols;
    
    const int rows_per_thread = (matrix_rows + MAX_THREADS - 1) / MAX_THREADS; // ceiling division
    
    int starting_row = thread_args->id * rows_per_thread;
    int ending_row = MIN(starting_row + rows_per_thread, matrix_rows);
    
    // printf("DEBUG: thread %d got rows %d to %d\n", thread_args->id, starting_row, ending_row);
    
    for (int i = starting_row; i < ending_row; ++i)
        for (int j = 0; j < matrix_cols; ++j)
            thread_args->t->data[i][j] = thread_args->m->data[i][j] + thread_args->n->data[i][j];
    
    return NULL;
}

/* Return the sum of two matrices. The result is in a newly creaed matrix. 
 *
 * If a pthread function fails, report error and exit. 
 * Return NULL if something else is wrong.
 *
 * Similar to addMatrix, but this function uses up to 2 threads.
 */
TMatrix *addMatrix_thread(TMatrix *m, TMatrix *n) {
    if (m == NULL || n == NULL || m->nrows != n->nrows || m->ncols != n->ncols )
        return NULL;
    
    TMatrix * t = newMatrix(m->nrows, m->ncols);
    if (t == NULL) return t;
    
    // create up to MAX_THREADS threads
    pthread_t threads[MAX_THREADS];
    thread_arg_t threads_arg_array[MAX_THREADS];
    const int num_actual_threads = MIN(MAX_THREADS, m->nrows);
    for (int i = 0; i < num_actual_threads; ++i) {
        threads_arg_array[i] = (thread_arg_t) {.id = i, m, n, t};
        assert(!pthread_create(&threads[i], NULL, thread_main, &threads_arg_array[i]));
    }
    
    // wait for threads
    for (int i = 0; i < num_actual_threads; ++i)
        assert(!pthread_join(threads[i], NULL));
    
    return t;
}
