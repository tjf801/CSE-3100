#include<stdio.h> 
#include <unistd.h> 
#include <fcntl.h> 

int main() 
{ 
	// open() returns a file descriptor fd to the file "dup.txt" 

	int fd = open("dup.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR); 
	
	if(fd < 0)
	{ 
		printf("Cannot open the file\n");
		return -1; 
	}
	// dup() will create the copy of fd as the copy_fd 
	// then both can be used interchangeably. 

	int copy_fd = dup(fd); 
		
	// write() will write the given string into the file 
	// referred by the file descriptors 

	write(copy_fd,"This will be output to the file named dup.txt\n", 46); 
		
	write(fd,"This will also be output to the file named dup.txt\n", 51); 

	close(copy_fd);
	close(fd);	
	return 0; 
} 

