#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>
#include <fcntl.h>

#define MAX 1024

int main(int argc, char *argv[]) {
    int count = 0;
    if (fork()) {
        printf("%p\n", &count);
        wait(NULL);
    } else {
        printf("%p\n", &count);
        count++;
    }
    
    printf("%d\n", count);
    return 0;
}