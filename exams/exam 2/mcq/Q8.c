#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int pd[2];
    assert(pipe(pd) != -1);
    
    pid_t pid = fork();
    if (pid == 0) {
        int i = 5;
        write(pd[0], &i, sizeof(i));
        close(pd[0]);
        return 0;
    }
    close(pd[1]);
    int k = 0;
    int n = read(pd[0], &k, sizeof(int));
    close(pd[0]);
    printf("n = %d\n", n);
    waitpid(pid, NULL, 0);
    return 0;
}