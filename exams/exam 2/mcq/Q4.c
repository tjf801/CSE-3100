#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>
#include <fcntl.h>

#define MAX 1024

int main(int argc, char *argv[]) {
    int pd[2];
    assert(pipe(pd) != -1);
    pid_t pid = fork();
    if (pid == 0) {
        close(pd[0]);
        char *msg = "Hello World!\nToday is a great day!\n";
        write(pd[1], msg, strlen(msg));
        close(pd[1]);
        return 0;
    }
    close(pd[1]);
    dup2(pd[0], STDIN_FILENO);
    close(pd[0]);
    char s[MAX];
    while (fgets(s, sizeof(s), stdin)) printf("%s", s);
    waitpid(pid, NULL, 0);
    return 0;
}