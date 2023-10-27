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
        char *msg = "Hello World!\n";
        close(pd[0]);
        for (int i = 0; i < strlen(msg); i++)
            write(pd[1], &msg[i], sizeof(char));
        close(pd[1]);
    }
    close(pd[1]);
    char c;
    do {
        read(pd[0], &c, sizeof(char));
        printf("%c", c);
    } while (c != '\n');
    close(pd[0]);
    waitpid(pid, NULL, 0);
    return 0;
}