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

#define     NAME_SIZE    64

typedef struct node_tag
{
    unsigned int    id;   
    char   name[NAME_SIZE];
} person_t;

// TODO
// implement a function to swap two (int *)
// add two paramters of correct types 
// the function does not return a value
// void    swap_pointer_int();

void test_swap_pointer_int(int argc, char *argv[])
{
    int     a = 10, b = 20;
    int     *pa = &a, *pb = &b;

    if (argc >= 2) 
        a = atoi(argv[1]);

    if (argc >= 3) 
        b = atoi(argv[2]);

    printf("Before swap\n");
    // you can print the value of pointers, if you like
    // but submission should not print pointers
    // printf(" pa is %p,  pb is %p\n",  pa,  pb);
    printf("*pa is %d, *pb is %d\n", *pa, *pb);

    // TODO 
    // one line to call swap_point_int() to swap pa and pb

    printf("After swap\n");
    // printf(" pa is %p,  pb is %p\n",  pa,  pb);
    printf("*pa is %d, *pb is %d\n", *pa, *pb);
}

// TODO
// implement a function to swap two structures of type person_t
// add two paramters of correct types 
// the function does not return a value
// void    swap_person();

void test_swap_person(int argc, char *argv[])
{
    // initialize structures  
    person_t   x = {1, "Alice"}, y = {2, "Bob"};

    if (argc >= 2) {
        strncpy(x.name, argv[1], NAME_SIZE - 1);
        x.name[NAME_SIZE - 1] = 0;
    }

    if (argc >= 3) {
        strncpy(y.name, argv[2], NAME_SIZE - 1);
        y.name[NAME_SIZE - 1] = 0;
    }

    printf("Before swap\n");
    printf("x's id is %d, x's name is %s\n", x.id, x.name);
    printf("y's id is %d, y's name is %s\n", y.id, y.name);

    // TODO 
    // one line to call swap_person() to swap x and y
    
    printf("After swap\n");
    printf("x's id is %d, x's name is %s\n", x.id, x.name);
    printf("y's id is %d, y's name is %s\n", y.id, y.name);
}

/* Do not change main() */
int main(int argc, char *argv[])
{
    if (argc == 1) {
        test_swap_pointer_int(argc, argv);
        test_swap_person(argc, argv);
    }
    else if (isdigit(argv[1][0]))  // if the first argument starts with a digit
        test_swap_pointer_int(argc, argv);
    else 
        test_swap_person(argc, argv);
    return 0;
}
