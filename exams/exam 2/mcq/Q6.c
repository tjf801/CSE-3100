#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    char *args[] = {"ls", NULL};
    execv(args[0], args);
    printf("This is bad!\n");
    return 0;
}