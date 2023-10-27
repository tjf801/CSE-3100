#ifndef RUN_SEARCH_H
#define RUN_SEARCH_H
//write an integer to a pipe
void write_int(int pd, int value);

//read an integer from a pipe
//the function returns the number of bytes read
int read_int(int pd, int *value);
int in_array(int a[], int n, int v);
void run_search(int n, int m);
#endif // RUN_SEARCH_H