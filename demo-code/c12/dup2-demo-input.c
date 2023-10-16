//dup2 demo
//Redirecting stand input to a file
#include<stdio.h> 
#include <unistd.h> 
#include <fcntl.h> 

#define MAX_LINE 80
int main() 
{
	char buffer[MAX_LINE];

 	
	int fd = open("dup2-input.txt", O_RDONLY); 
	
	if(fd < 0)
	{ 
		printf("Error opening the file\n");
		return -1;
	} 
	
	dup2(fd, 0);
	close(fd);

	//repeatly read from standard input
	//and print to the standard output
	while(fgets(buffer, MAX_LINE, stdin))
		printf("%s", buffer);
	return 0; 
} 

