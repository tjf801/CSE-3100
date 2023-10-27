#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>
#include <fcntl.h>

#define MAX 1024

int main(int argc, char *argv[]) {
    printf("first line\n");
    int fd = open("dup2-output.txt", O_WRONLY | O_CREAT, 0644);
    if (fd < 0) {
        printf("Error opening file.\n");
        return -1;
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
    printf("second line\n");
    return 0;
}