#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include "matrix.h"

#define MAX_THREADS 2
#define BLOCK_SIZE 8
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

typedef struct {
    unsigned int id;
    TMatrix *m, *n_T, *t;
} thread_arg_t;

static void *thread_main(thread_arg_t *args) {
    const int num_block_rows = (args->m->nrows + BLOCK_SIZE - 1) / BLOCK_SIZE;
    const int num_block_rows_per_thread = num_block_rows / MAX_THREADS;
    
    const int start_m_row = args->id * num_block_rows_per_thread * BLOCK_SIZE;
    
    for (int block_row_num = 0; block_row_num < num_block_rows_per_thread; block_row_num++) {
        for (int block_start_n_col = 0; block_start_n_col < args->n_T->nrows; block_start_n_col += BLOCK_SIZE) {
            int block_start_row = start_m_row + BLOCK_SIZE * block_row_num;
            for (int i = block_start_row; i < block_start_row + BLOCK_SIZE && i < args->m->nrows; ++i) {
                for (int j = block_start_n_col; j < block_start_n_col + BLOCK_SIZE && j < args->n_T->nrows; ++j) {
                    // dot product
                    TElement sum = (TElement)0;
                    for (int k = 0; k < args->m->ncols; ++k)
                        sum += args->m->data[i][k] * args->n_T->data[j][k];
                    args->t->data[i][j] = sum;
                }
            }
        }
    }
    
    return NULL;
}

/* Return the product of two matrices.
 *
 * If any pthread function fails, report error and exit. 
 * Return NULL if anything else is wrong.
 *
 * Similar to mulMatrix, but with multi-threading.
 */
TMatrix * mulMatrix_thread(TMatrix *m, TMatrix *n) {
    if (m == NULL || n == NULL || m->ncols != n->nrows) return NULL;
    
    TMatrix * t = newMatrix(m->nrows, n->ncols);
    if (t == NULL) return t;
    // memset(t->data, 0, t->nrows* t->ncols * sizeof(TElement));
    
    // transposing n is much better for cache performance!!!
    TMatrix * n_T = transposeMatrix(n);
    if (n_T == NULL) return t;
    
    // create up to MAX_THREADS threads
    pthread_t threads[MAX_THREADS];
    thread_arg_t threads_arg_array[MAX_THREADS];
    const int num_actual_threads = MIN(MAX_THREADS, m->nrows);
    for (int i = 0; i < num_actual_threads; ++i) {
        threads_arg_array[i] = (thread_arg_t) {.id = i, m, n_T, t};
        assert(!pthread_create(&threads[i], NULL, (void *(*)(void *))thread_main, &threads_arg_array[i]));
    }
    
    // wait for threads
    for (int i = 0; i < num_actual_threads; ++i)
        assert(!pthread_join(threads[i], NULL));
    
    return t;
}
