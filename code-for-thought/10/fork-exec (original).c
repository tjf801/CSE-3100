//In this code-for-thought problem, we practice fork() and execv(). 
//Specifically, we complete the code for fork-exec.c to fork a child process which
//runs a command like "./sum 100" to calculate the sum of the
//first 100 postive integers.
//The source code sum.c is provided. No change is needed.
//Moreover, here 100 should be a command line argument for fork-exec
//For example, after the code for fork-exec.c is completed and complied
//We type ./fork-exec 100 to run this program and this program will 
//create a child process to run ./sum 100


#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <sys/wait.h> 

int main(int argc, char *argv[])
{
	if(argc!=2)
	{
		//command line needs to take an argument n
		printf("%s n\n", argv[0]);
		return -1;
	}

    pid_t pid; 
    int status; 
    pid = fork(); 
    if (pid == -1)
	{ 

        // pid == -1 means error occured 
        printf("Cannot fork, error occured\n"); 
        exit(EXIT_FAILURE); 
    } 
    else if (pid == 0)
	{ 

        // pid == 0 means child process created 
        // getpid() returns process id of calling process 
        //printf("child process, pid = %u\n",getpid()); 

        // the argv list first argument should point to 
        // filename associated with file being executed 
        // the array pointer must be terminated by NULL 
        // pointer

		//TODO:
		//fill in the code below 
        char * argv_list[] = {//fill in your code here}.
    
        // the execv() only return if error occured. 
        // The return value is -1
		//TODO:
		//fill in the code below 
        execv(//fill your code here); 
        exit(-1); 
    } 
    else
	{ 

        // the parent process calls waitpid() on the child 
        // waitpid() system call suspends execution of 
        // calling process until a child specified by pid 
        // argument has changed state 
        // see wait() man page for all the flags or options 
        // used here 
        if (waitpid(pid, &status, 0) > 0) 
		{ 
            
            if (WIFEXITED(status) && !WEXITSTATUS(status)) 
	            printf("Program execution successfull\n"); 
            
            else if (WIFEXITED(status) && WEXITSTATUS(status)) 
			{ 
                if (WEXITSTATUS(status) == 127) 
				{ 

                    // execv failed 
                    printf("Execv failed\n"); 
                } 
                else
                    printf("Program terminated normally,"
                    " but returned a non-zero status\n");                 
            } 
            else
            	printf("Program did not terminate normally\n");             
        } 
        else 
		{ 
        	// waitpid() failed 
            printf("waitpid() failed\n"); 
        } 
        exit(0); 
    } 
    return 0; 
} 

