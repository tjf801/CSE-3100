#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>
#include <fcntl.h>

#define MAX 1000000

int main(int argc, char *argv[]) {
    int pd[2];
    assert(pipe(pd) != -1);
    close(pd[1]);
    pid_t pid = fork();
    if (pid == 0) {
        for (int i = 0; i < MAX; i++)
            write(pd[1], &i, sizeof(i));
        close(pd[1]);
        return 0;
    }
    int count = 0, k;
    for (int i = 0; i < MAX; i++)
        if (read(pd[0], &k, sizeof(int))) count++;
    close(pd[0]);
    printf("count = %d\n", count);
    waitpid(pid, NULL, 0);
    return 0;
}