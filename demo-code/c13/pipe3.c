#include <stdio.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) 
{ 
    int pd[2];
    if(pipe(pd) == -1)
    {
        perror("Error.");
        return -1;
    }

    pid_t pid = fork(); 
    if(pid == 0)
    {
        close(pd[0]);
        dup2(pd[1], 1);
        close(pd[1]);
        char * argv_list[] = {"cat", "pipe3.c", NULL};
        execvp(argv_list[0], argv_list); 
    }
    close(pd[1]);

    int pd1[2];
    if(pipe(pd1) == -1)
    {
        perror("Error.");
        return -1;
    }
    
    pid_t pid1 = fork();
    if(pid1 == 0)
    {
        close(pd1[0]);
        dup2(pd[0], 0);
        close(pd[0]);
        dup2(pd1[1], 1);
        close(pd1[1]);
        char * argv_list[] = {"tr", "[a-z]", "[A-Z]", NULL};
        execvp(argv_list[0], argv_list);
    }
    close(pd[0]);
    close(pd1[1]);

    pid_t pid2 = fork();
    if(pid2 == 0)
    {
        dup2(pd1[0], 0);
        close(pd1[0]);
        char * argv_list[] = {"tail", NULL};
        execvp(argv_list[0], argv_list);
    }
	close(pd1[0]);
	
    waitpid(pid, NULL, 0);
    waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return 0; 
} 

