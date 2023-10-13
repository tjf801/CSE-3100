#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s cmd1 cmd1_arg cmd2 [cmd2_args ..]\n", argv[0]);
        return 1;
    }
    
    // making this program made me miss rust's `Result<T, E>`.
    
    // child process runs first command
    pid_t cmd1_runner_pid = fork();
    if (cmd1_runner_pid == -1) {
        perror("fork()");
        exit(EXIT_FAILURE);
    }
    
    if (cmd1_runner_pid == 0) {
        // in child process - execute first command
        char *args[] = {argv[1], argv[2], NULL};
        if (execvp(args[0], args) == -1) {
            perror("execvp()");
            exit(EXIT_FAILURE);
        }
    }
    
    // still in parent process, so wait for first command to finish
    int status_1;
    pid_t err1 = waitpid(cmd1_runner_pid, &status_1, 0);
    if (err1 == -1) {
        perror("waitpid()");
        exit(EXIT_FAILURE);
    }
    printf("exited=%d exitstatus=%d\n", WIFEXITED(status_1), WEXITSTATUS(status_1));
    
    // child process runs second command
    pid_t cmd2_runner_pid = fork();
    if (cmd2_runner_pid == -1) {
        perror("fork()");
        exit(EXIT_FAILURE);
    }
    
    if (cmd2_runner_pid == 0) {
        // in child process, so run second command
        if (execvp(argv[3], &argv[3]) == -1) {
            perror("execvp()");
            exit(EXIT_FAILURE);
        }
    }
    
    // still in parent process, so wait for cmd 2 to finish
    int status_2;
    pid_t err2 = waitpid(cmd2_runner_pid, &status_2, 0);
    if (err2 == -1) {
        perror("waitpid()");
        exit(EXIT_FAILURE);
    }
    printf("exited=%d exitstatus=%d\n", WIFEXITED(status_2), WEXITSTATUS(status_2));
    
    return 0;
}
