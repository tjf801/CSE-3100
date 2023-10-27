#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <fcntl.h>
#include <sys/stat.h>

//redirect standard input to the specified file
void redirectStdin(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("Error opening the file\n");
        exit(-1);
    }
    if (dup2(fd, STDIN_FILENO) < 0) {
        perror("Error redirecting stdin\n");
        exit(-1);
    }
}

//redirect standad output to the specified file
void redirectStdout(const char *filename) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        perror("Error opening the file\n");
        exit(-1);
    }
    if (dup2(fd, STDOUT_FILENO) < 0) {
        perror("Error redirecting stdout\n");
        exit(-1);
    }
}

//if word has letters in strict alphabetical order, return 1 else return 0
int increasing(char *word) {
    size_t n = strlen(word);
    if (n < 5) return 0;
    for (int i = 0; i < n - 1; i++)
        if (word[i] >= word[i + 1])
            return 0;
    return 1;
}
