//Can you see from the example that following points on fork() from the slides are valid 
//Cannot assume that they return at the same time or which one “returns first”
// (even on a uni-core)
//Order is chosen by OS scheduler

//All files open in the parent are accessible in the child!
//I/O operations in either one move the file position indicator
//Any memory change (stack/heap) affects only the copy in that process

#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
 
int main(int argc, char *argv[]) 
{

    if(argc!= 3)
    {
        printf("Usage: %s n result_file\n", argv[0]);
        return -1;
    }
    int n = atoi(argv[1]);
    assert(n>=1 && n<=10);
    FILE *fp = fopen(argv[2], "w");
    if(fp==NULL)
    {
        printf("Cannot open the file %s", argv[2]);
        return -1;
    }

    int count = 0;
    pid_t pid;

    for(int i=0; i<n; i++)
    { 
        pid = fork();
        if(pid == 0)
        {
            //increase count in the child process
            count ++;
        }
    } 
    fprintf(fp, "count and pid : %d %d\n", count, getpid());
    fclose(fp);
    //use wc command to count the number of lines in the result_file and think why
    //we get that number. Note each process writes one line in the file. 
    return 0; 
} 

