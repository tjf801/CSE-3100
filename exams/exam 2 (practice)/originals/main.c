#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "pipe-sort.h"

#define MAX_N 100000

void parse_command_line(int argc, char *argv[], int *seed, int *n, int *print_sorted, int *num_printed);

int main(int argc, char *argv[])
{
    int seed = 3100, n, print_sorted = 1, num_printed = 0;

    // parse command line arguments
    parse_command_line(argc, argv, &seed, &n, &print_sorted, &num_printed);

    fprintf(stderr,"seed=%d n=%d print_sorted=%d num_printed=%d\n", seed, n, print_sorted, num_printed);

    // call the pipe sort implementation
    pipe_sort(seed, n, print_sorted, num_printed);

    return 0;
}


void parse_command_line(int argc, char *argv[], int *seed, int *n, int *print_sorted, int *num_printed)
{
    int flag = 0;

    for (int i = 1; i < argc; i++) {
        if (! strcmp(argv[i], "-u")) {
            *print_sorted = 0;
        } else if (strncmp(argv[i], "-s", 2) == 0) {
            *seed = atoi(&argv[i][2]);
            if (*seed <= 0) {
                fprintf(stderr, "seed must be a positive integer: %s\n", &argv[i][2]);
                exit(EXIT_FAILURE);
            }
        } else if (strncmp(argv[i], "-p", 2) == 0) {
            *num_printed = atoi(&argv[i][2]);
            if (*num_printed < 0) {
                fprintf(stderr, "number of integers to be printed must be a non-negative integer: %s\n", &argv[i][2]);
                exit(EXIT_FAILURE);
            }
        } else if (isdigit(argv[i][0])) {
            *n = atoi(argv[i]);
            if (*n <= 0 || *n > MAX_N || *n % 2) {
                fprintf(stderr, "The number of elements must be a postive even integer <= %d.\n", MAX_N);
                exit(EXIT_FAILURE);
            }
            flag = 1;
        } else {
            break;
        }
    }
    if (!flag) {
        fprintf(stderr, "Usage: %s <N> [-s<N>] [-p<N>] [-b]\n"
                "<N>     : number of integers to sort. Must be specified.\n"
                "-s<N>   : seed. Default value is 3100.\n"
                "-p<N>   : number of integers to print. Default value is 0, which prints all.\n"
                "-u      : print the numbers before sorting and exit.\n"
                , argv[0]);
        exit(EXIT_FAILURE);
    }
}