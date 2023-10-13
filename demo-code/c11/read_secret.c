#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX 1024

//read from a file a encrypted message and then decrypt and print the message
void read_secret_message(char key, int fd)
{
	int message[MAX];
	int len = read(fd, message, MAX);
    	for(int i = 0; i<len/sizeof(int); i++)
    	{
        	message[i] = message[i] ^ key;
		printf("%c", (char)(message[i]));      
    	}	
	printf("\n");
}

int main()
{
	int fd = open("message.txt", O_RDONLY);
	if(fd < 0)
	{
		printf("Cannot open the file\n");
        	return -1;
	}
	read_secret_message(111, fd);
	close(fd);
}
