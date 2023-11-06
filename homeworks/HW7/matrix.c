#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "matrix.h"

// For the purpose of this assignment
#define     EPSILON     0.0000001

/*
 * Creates and returns a matrix of size rows x cols
 *
 * If the allocation is not successful, the function should return NULL.
 * If the allocation is successful, the data field of the matrix should
 * point to an array of pointers (representing the rows) and each pointer
 * in that array should point to an array of TElement representing the values
 * in that row.
 *
 * Paraterms:
 *   nrows : (non-negative value) giving the number of rows
 *   ncols : (non-negative value) giving the number of columns
 *
 * Return values:
 *   NULL: Failed to create the matrix.
 *   Otherwise, a pointer to the created matrix.
 */
TMatrix * newMatrix(unsigned int nrows, unsigned int ncols)
{
    if (! nrows || ! ncols)
        return NULL;

    /* Allocate space for TMatrix */
    TMatrix * newM = malloc(sizeof(TMatrix));

    /* Allocate space for a rows X cols matrix
       allocate pointers to row arrays      */
    TElement ** data = malloc(nrows * sizeof(TElement *));

    /* allocate space for all elements in one call */
    TElement * storage = malloc (nrows*ncols * sizeof(TElement));

    if (newM == NULL || data == NULL || storage == NULL) {
        free(newM);
        free(data);
        free(storage);
        return NULL;
    }

    /* set row array pointers */
    for (size_t i = 0; i < nrows; i++)
            data[i] = storage + i * ncols;
    newM->nrows = nrows;
    newM->ncols = ncols;
    newM->data = data;
    return newM;
}

/*
 * This function is responsible for deallocating the dynamic memory
 * currently used by a matrix. Check the newMatrix() function to see
 * what were allocated.
 */
void freeMatrix(TMatrix * m)
{
    if (m == NULL)          /* remember to check in every function */
        return;
    if (m->data)  {
        free(m->data[0]);   /* free the storage allocated for data */
        free(m->data);      /* free the array of pointers to row pointers */
    }
    free(m);                /* free the matrix itself */
}

/*
 * This function fill the matrix with random numbers. 
 * This function returns the parameter, a pointer to matrix 
 */
TMatrix * fillMatrix(TMatrix * m)
{
    static int first=1;

    if (m == NULL || m->data == NULL)
        return m;

    // again, for the purpose of this assignment.
    if (first) {
        srand(3100);
        first = 0;
    }

    unsigned int i, j;
    for (i = 0; i < m->nrows; i ++)
        for (j = 0; j < m->ncols; j ++)
            m->data[i][j] = (TElement) rand();
    return m;
}

/*
 * The transposeMatrix function takes as input a matrix m and returns a
 * new matrix that holds the transpose of m. Transposition should run in
 * O(nrows x ncols)  (where n is the # of rows and m the # of columns).
 * If memory allocation for the transpose failed or input is NULL,
 * the function returns NULL.
 * Transposition follows the usual mathematical definition of transposition.
 */
TMatrix * transposeMatrix(TMatrix * m)
{
    if (m == NULL)
        return NULL;
    TMatrix * t = newMatrix(m->ncols, m->nrows);
    if (t == NULL)
        return t;
    for (unsigned int i = 0; i < m->nrows; i++)
        for (unsigned int j = 0; j < m->ncols; j++)
            t->data[j][i] = m->data[i][j];
    return t;
}

/* This function prints the elements in a matrix m.  
 * Numbers in a row are printed on the same line.
 * Apparently, it is hard to read if the matrix is large. 
 * 
 * This function does not return a value.
 */
void printMatrix(TMatrix * m)
{
    if (m == NULL)
        return;
    for (unsigned int i = 0; i < m->nrows; i++) {
        for (unsigned int j = 0; j < m->ncols; j++)
            printf("%12g", m->data[i][j]);
        printf("\n");
    }
}

/* This function compares two matrices m and n.
 *
 * Return values:
 *  0:  if m and n are the same (difference is less than EPSILON) 
 *  1:  m and n are different. The first different elements are printed to stdout.
 *  -1: m or n is NULL. 
 *  -2: m and n have different dimensions.
 */
int compareMatrix(TMatrix * m, TMatrix *n)
{
    if (m == NULL || n == NULL)
        return -1;
    if (m->nrows != n->nrows || m->ncols != n->ncols)
        return -1;

    unsigned i, j;
    for (i = 0; i < m->nrows; i ++)
        for (j = 0; j < m->ncols; j ++)
            if (fabs(m->data[i][j] - n->data[i][j]) > EPSILON) {
                fprintf(stderr, "element[%u][%u]  %f  %f.\n", i, j, m->data[i][j], n->data[i][j]);
                return 1;
            }
    return 0;
}

/* This function adds two matrices m and n. 
 * It returns the sum of two matrices, which is created in this function. 
 *
 * Return values:
 *      NULL if anything is wrong.
 *      Otherwise, a pointer to the result, a new matrix created in this function.
 */
TMatrix * addMatrix(TMatrix *m, TMatrix *n)
{
    if (    m == NULL || n == NULL
         || m->nrows != n->nrows || m->ncols != n->ncols )
        return NULL;

    TMatrix * t = newMatrix(m->nrows, m->ncols);
    if (t == NULL)
        return t;
    for (unsigned int i = 0; i < m->nrows; i++)
        for (unsigned int j = 0; j < m->ncols; j++)
            t->data[i][j] = m->data[i][j] + n->data[i][j];
    return t;
}

/* This function multiplies two matrices m and n. 
 * It returns the product of two matrices, which is created in this function. 
 *
 * Return values:
 *      NULL if anything is wrong.
 *      Otherwise, a pointer to the result, a new matrix created in this function.
 */
TMatrix * mulMatrix(TMatrix *m, TMatrix *n)
{
    if (     m == NULL || n == NULL
          || m->ncols != n->nrows   )
        return NULL;

    TMatrix * t = newMatrix(m->nrows, n->ncols);
    if (t == NULL)
        return t;
    for (unsigned int i = 0; i < m->nrows; i++)  {
        for (unsigned int j = 0; j < n->ncols; j++) {
            TElement sum = (TElement)0;
            for (unsigned int k = 0; k < m->ncols; k++)
                sum += m->data[i][k] * n->data[k][j];
            t->data[i][j] = sum;
        }
    }
    return t;
}
