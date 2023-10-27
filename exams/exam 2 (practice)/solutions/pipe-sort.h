#ifndef PIPESORT_H
#define PIPESORT_H

void die(char *s);

void print_array(int arr[], int n, int upto);

int compare_int(const void *a, const void *b);

void merge(int a[], int b[], int c[], int n);

void pipe_sort(int seed, int n, int print_sorted, int num_printed);

#endif