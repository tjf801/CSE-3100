#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int a = 0;
    pid_t pid = fork();
    if (pid == 0) a++;
    a--;
    if (pid) wait(NULL);
    printf("a = %d\n", a);
    return 0;
}