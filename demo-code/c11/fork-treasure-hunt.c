#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    
    if(argc!=2)
    {
        printf("Usage: %s n\n", argv[0]);
        return -1;
    }
    
    int n = atoi(argv[1]);
    assert(n >= 1 && n<=10);

    int *a = calloc(n, sizeof(int));

    srand(12345);    
    int idx = rand() % n;
    pid_t pid = getpid();
    printf("Treasure hidden at %d in array %p pid = %d\n", idx, a, pid);
    a[idx] = 1;

    int cur = 0;

    for(int i = 0; i< n; i++)
    {
		pid_t pid;
    	pid = fork();
        if(pid == 0)
        {
            if(a[cur])
            {
                printf("Systematic search found the treasure at %d in array %p pid = %d\n", cur, a, getpid());
                break;;
            }
            cur ++;
        }
        else
        {
			//wait for the child process to finish
			waitpid(pid, NULL, 0);			
            int guess = rand() % n;
            if(a[guess])
            {
                printf("Random search found the treasure at %d in array %p pid = %d\n", guess, a, getpid());
                break;;
            }    
        }
    }
    free(a);
    return 0;
}
