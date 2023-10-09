//When you run the complied program, do not freak out
//Your computer still works
//There is no need to press ctrl + c :)
//Just press ENTER, and your computer will be normal again.
#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>

int main() 
{
    pid_t pid1 = fork(); 
    pid_t pid2 = fork(); 
    pid_t pid3 = fork(); 
    printf("pid1 = %d pid2 = %d pid3 = %d pid = %d\n", pid1, pid2, pid3, getpid());
    return 0; 
} 

