//In this example, we demonstrate how to use pipe, write and read.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

//Calculate factorial
int factorial(int n)
{
    int result = 1;
    for(int i = 1; i<= n; i++)
        result *= i;
    
    return result;
}

//Write to a pipe
void write_solution(int solution, int pd)
{
    write(pd, &solution, sizeof(solution));
}

//Read from a pipe
void read_solution(int pd, int *solution)
{
    read(pd, solution , sizeof(int));
}

int main(int argc, char *argv[])
{
    if(argc!=2)
    {
        printf("Usage: %s n\n", argv[0]);
        return -1;
    }
    int pd[2];

    if(pipe(pd) == -1)
    {
        perror("Error.");
        return -1;
    }
    pid_t pid = fork();
    if(pid == 0)
    {
        //We close pd[0] since the child process does not need to read from the pipe
        close(pd[0]);
        int n = atoi(argv[1]);
        write_solution(factorial(n), pd[1]);
        //close pd[1] after write
        close(pd[1]);
        return 0;
    }
    //Only the parent process will run the following code
	//We close pd[1] since the parent process does not need to write to the pipe	
	close(pd[1]);
	int result;
	read_solution(pd[0], &result);
	printf("result = %d\n", result);
	//We close pd[0] since the parent process no longer needs to read from the pipe
	close(pd[0]);
	waitpid(pid, NULL, 0);
	return 0;
}
