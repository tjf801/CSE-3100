//In this exmple, we demo how to redirect a child process's standard output
//Moreover, the child process does exec to run a command line
//wc filename
//to count the # of lines and word count of the file 
//The standard output is redicted to output.txt

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <fcntl.h> 
#include <ctype.h> 
#include <sys/stat.h>
#define MAX_LINE 1024

int main(int argc, char *argv[])
{

	if(argc!=2)
	{
		printf("Usage: %s filename\n", argv[0]);
		return -1; 
	}
	pid_t pid;
	pid = fork();
	if(pid == 0)
	{

		//Make sure when the file is created, the user has the read and write access 
		int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR); 
		if(fd < 0)
		{ 
			printf("Cannot open the file\n");
			return -1;
		} 
		dup2(fd, 1);
		close(fd);
        	char * argv_list[] = {"wc", argv[1], NULL}; 
    
        	// the execv() only return if error occured. 
        	// The return value is -1 
        	execvp("wc",argv_list); 
		printf("Something is wrong.\n");
		exit(-1);
	}

	return 0;
} 

