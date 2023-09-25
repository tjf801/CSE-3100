/*  In this assignment, we experiment with passing references to functions

    On the following web site, you can find a swap funciton that swaps two integers.
    https://www.tutorialspoint.com/cprogramming/c_function_call_by_reference.htm

    In this exercise, we implement two functions. 
    One function swaps two pointers to int,  and 
    the other swaps structures. 

    Search TODO to find the locations where we need to work 
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#define NAME_SIZE 64

typedef struct node_tag {
    unsigned int id;   
    char name[NAME_SIZE];
} person_t;

#define SWAP_PTR_TEMPLATE(T) (T * const a, T * const b) { *a ^= *b; *b ^= *a; *a ^= *b; }
static void swap_pointers_char SWAP_PTR_TEMPLATE(char)
static void swap_pointers_uchar SWAP_PTR_TEMPLATE(unsigned char)
static void swap_pointers_short SWAP_PTR_TEMPLATE(short)
static void swap_pointers_ushort SWAP_PTR_TEMPLATE(unsigned short)
static void swap_pointers_int SWAP_PTR_TEMPLATE(int)
static void swap_pointers_uint SWAP_PTR_TEMPLATE(unsigned int)
static void swap_pointers_long SWAP_PTR_TEMPLATE(long)
static void swap_pointers_ulong SWAP_PTR_TEMPLATE(unsigned long)
static void swap_pointers_ll SWAP_PTR_TEMPLATE(long long)
static void swap_pointers_ull SWAP_PTR_TEMPLATE(unsigned long long)

// ideally this would be generic over whatever struct you pass in
// but honestly i dont care enough to figure all of that out
static void swap_pointers_person(person_t * const a, person_t * const b) {
    // swap the ids
    swap_pointers_uint(&a->id, &b->id);
    // swap the names
    for (int i = 0; i < NAME_SIZE; ++i)
        swap_pointers_char(&a->name[i], &b->name[i]);
}

#define swap_pointers(a, b) _Generic((a), \
    char*: swap_pointers_char, unsigned char*: swap_pointers_uchar, \
    short*: swap_pointers_short, unsigned short*: swap_pointers_ushort, \
    int*: swap_pointers_int, unsigned int*: swap_pointers_uint, \
    long*: swap_pointers_long, unsigned long*: swap_pointers_ulong, \
    long long*: swap_pointers_ll, unsigned long long*: swap_pointers_ull, \
    person_t*: swap_pointers_person \
)(a, b)

void test_swap_pointers_int(int argc, char *argv[]) {
    int a = 10, b = 20;
    int *pa = &a, *pb = &b;
    
    if (argc >= 2) a = atoi(argv[1]);
    if (argc >= 3) b = atoi(argv[2]);
    
    printf("Before swap\n");
    printf("*pa is %d, *pb is %d\n", *pa, *pb);
    
    swap_pointers(pa, pb);
    
    printf("After swap\n");
    printf("*pa is %d, *pb is %d\n", *pa, *pb);
}

void test_swap_person(int argc, char *argv[]) {
    person_t x = {1, "Alice"}, y = {2, "Bob"};
    
    if (argc >= 2) strncpy(x.name, argv[1], NAME_SIZE);
    if (argc >= 3) strncpy(y.name, argv[2], NAME_SIZE);
    
    printf("Before swap\n");
    printf("x's id is %d, x's name is %s\n", x.id, x.name);
    printf("y's id is %d, y's name is %s\n", y.id, y.name);
    
    swap_pointers(&x, &y);
    
    printf("After swap\n");
    printf("x's id is %d, x's name is %s\n", x.id, x.name);
    printf("y's id is %d, y's name is %s\n", y.id, y.name);
}

/* Do not change main() */
int main(int argc, char *argv[]) {
    if (argc == 1) {
        test_swap_pointers_int(argc, argv);
        test_swap_person(argc, argv);
    }
    
    else if (isdigit(argv[1][0]))  // if the first argument starts with a digit
        test_swap_pointers_int(argc, argv);
    
    else test_swap_person(argc, argv);
    
    return 0;
}
