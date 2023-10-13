#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

//encrypt the message and write the result to file fd
void write_secret_message(char *message, char key, int fd)
{
	int len = strlen(message);
	printf("len = %d\n", len);
	int msg[len];

	for(int i = 0; i<len; i++)
	{
		msg[i] = message[i] ^ key;
		//printf("%c", msg[i]);		
	}
	write(fd, msg, len*sizeof(int));
	printf("%s\n", (char *)msg);
}

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("Usage: %s message('I love you!')\n", argv[0]);
		return -1;
	}
	//note how we open the file
	int fd = open("message.txt", O_WRONLY | O_TRUNC | O_CREAT, 0600);
	if(fd < 0)
	{
		printf("Cannot open the file\n");
		return -1;
	}
	write_secret_message(argv[1], 111, fd);
	//remember to close the file
	close(fd);
	return 0;
}
