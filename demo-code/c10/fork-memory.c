#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
    int a = 0;
    if (fork() == 0) { 
        a++; 
        printf("Child process: a = %10d, &a = %p\n", a, &a); 
    } 
    else { 
        a--; 
        printf("Parent process: a = %10d, &a = %p\n", a, &a); 
    }
    return 0;
}
