//In this assignment, we practice dup2() and fork().
//The main() function takes a command line argument.
//In the main() function we use fork() to create a
//child process. The child process redirects the
//standard output to a file named "output.txt" using dup2().
//Moreover, it prints out the command line argument in capital letters.
//The parent process simply prints out the command line argument. 

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <fcntl.h> 
#include <ctype.h> 
#include <sys/stat.h>
// #define MAX_LINE 1024 // not used

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s message\n", argv[0]);
        return -1; 
    }
    
    if (fork() == 0) {
        //Make sure when the file is created, the user has the read and write access 
        int output_fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (output_fd < 0) { 
            printf("Cannot open the file\n");
            return -1;
        }
        
        // // this line is literally a buffer overflow vulnerability but ok
        // char buffer[MAX_LINE];
        // strcpy(buffer, argv[1]);
        
        // convert argv[1] to caps
		// and yes, i just checked, and modifying argv is completely defined behavior, apparently
        for (int i = 0; argv[1][i]; ++i) argv[1][i] = toupper(argv[1][i]);
        
        // redirect stdout to the file
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);
    }
    
    printf("%s\n", argv[1]);
    return 0;
} 

