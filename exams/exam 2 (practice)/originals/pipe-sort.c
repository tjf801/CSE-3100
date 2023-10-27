#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#include "pipe-sort.h"

#define MAX_N 100000
#define PFD_READ     0
#define PFD_WRITE    1

/*************  error handling functions  ****************/
void die(char *s)
{
    if (errno)
        perror(s);
    else
        fprintf(stderr, "Error: %s\n", s);
    exit(EXIT_FAILURE);
}

// print numbers in array arr
// n is the number of elements in arr
// upto is the number of elements to be printed
// if upto is 0, all elements are printed
void print_array(int arr[], int n, int upto)
{
    if (upto == 0 || n < upto)
        upto = n;
    for(int i=0; i<upto; i++) 
        printf("%d\n", arr[i]);
}

// This function is the compare function used by the qsort()
int compare_int(const void *a, const void *b)
{
    return *((int *)a) - *((int *)b);
}

// merge the sorted arrays a[] and b[] to a sorted array c[]
// both a[] and b[] have n integers
// all arrays are sorted in increasing order 
void merge(int a[], int b[], int c[], int n)
{
    int i, j, k;

    i = j = k = 0;

    while (i < n || j < n)
    {
        if (j == n || (i < n && a[i] < b[j])) {
            c[k++] = a[i];
            i++;
        } else {
            c[k++] = b[j];
            j++;
        }
    }
}

// write an integer to a pipe
void write_int(int pd, int value)
{
    if (write(pd, &value, sizeof(int)) != sizeof(int))
        die("write()");
}

//read an integer from a pipe
//the function returns the number of bytes read
int read_int(int pd, int *value)
{
    return read(pd, value, sizeof(int));
}



void pipe_sort(int seed, int n, int print_sorted, int num_printed){

    srand(seed);    // set the seed

    // prepare arrays
    // u has all the integers to be sorted
    // a is the first half and b is the second half
    int u[n];
    int *a, *b;
    int half = n / 2;

    a = u;
    b = a + half;

    for (int i = 0; i < n; i++)
        u[i] = rand() % n;

    if (! print_sorted) {
        print_array(u, n, num_printed);
        fprintf(stderr, "The unsorted array has been printed to stdout.\n"); 
        exit(EXIT_SUCCESS);
    }

    int pd1[2], pd2[2];

    // create pipes
    if(pipe(pd1) == -1)
        die("pipe() 1");

    if(pipe(pd2) == -1)
        die("pipe() 2");

    // TODO
    // create 2 child processes to sort arrays a and b, into increasing order
    //
    //  child 1:  
    //      close file descriptors that are not needed
    //      call qsort() to sorts a, 
    //      writes sorted integers to pipe 1 (pd1)
    //      close file descriptor(s) and exit
    //
    //  child 2:  
    //      close file descriptors that are not needed
    //      call qsort() to sorts b, 
    //      writes sorted integers to pipe 2 (pd2)
    //      close file descriptor(s) and exit
    //
    // The parent process reads sorted integers from child processes. 
    //      Results from child 1 are saved in a. 
    //      Results from child 2 are saved in b. 
    // Parent should wait for child processes.
    //
    // If any of read(), write(), or fork() fails, 
    //      report error and exit from the process
    //
    // All processes should close file descriptors that are not needed.

    int sorted[n];
    merge(a, b, sorted, half);
    if (print_sorted)
        print_array(sorted, n, num_printed);
}
