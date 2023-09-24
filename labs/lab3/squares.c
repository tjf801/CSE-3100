#include <stdio.h>

/* This program should print the sum of the elements
 *     1^2, 2^2, 3^2, ..., n^2
 * where n is an integer provided by the user on the
 * command line. Hunt down the bugs and squash them!
 * Seek out the memory leaks and plug them up! */

/* Computes the sum of the first n elements in the array. */
unsigned int sum(const unsigned int n, const unsigned int * const arr) {
    unsigned int sum = 0;
    for (unsigned int i = 0; i <= n; i++)
        sum += arr[i];
    return sum;
}

/* Fills the given array with the values
 * 1^2, 2^2, 3^2, ..., (n-1)^2, n^2. */
void fillSquares(const unsigned int n, unsigned int * const arr) {
    for (unsigned int i = 0; i <= n; i++)
        arr[i] = i*i;
}

/* Reads an integer n from arguments,
 * fills array with squares, and computes
 * the sum of the squares. Prints out the
 * sum before freeing all used memory. */
int main(int argc, char *argv[]) {
    // There must be at least 1 argument after the program
    // although only the first one is used.
    if(argc < 2) {
        printf("usage: ./squares n\n");
        return 1;
    }
    
    // convert the first argument from a string to a number
    const unsigned int n = atoi(argv[1]);
    if (n <= 0) {
        printf("n must be positive.");
        return 1;
    }
    
    unsigned int * const arr = (unsigned int *)malloc((n+1) * sizeof(unsigned int));
    fillSquares(n, arr);
    
    const unsigned int total = sum(n, arr);
    printf("total: %d\n", total);
    
    free(arr);
    
    return 0;
}
