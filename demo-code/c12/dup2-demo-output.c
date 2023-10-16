//dup2 demo
//redirecting stand output to a file
#include<stdio.h> 
#include <unistd.h> 
#include <fcntl.h> 

int main() 
{
	
	printf("Before dup2.\n");
	printf("Can you see this on the screen?\n");

	//Make sure when the file is created, the user has the read and write access 
	int fd = open("dup2-output.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR); 
	
	if(fd < 0)
	{ 
		printf("Error opening the file\n");
		return -1;
	} 
	
	printf("Right before dup2.\n");
	dup2(fd, 1);
	close(fd); 
	printf("After dup2.\n");
	printf("Can you see this on the screen? I guess not.\n");
	return 0; 
} 

