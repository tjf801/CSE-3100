#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>
#include <sys/types.h>

#define PIPEFD_READ     0
#define PIPEFD_WRITE    1

//write an integer to a pipe
void write_int(int pd, int value) {
    write(pd, &value, sizeof(int));
}

//read an integer from a pipe
//the function returns the number of bytes read
int read_int(int pd, int *value) {
    return read(pd, value , sizeof(int));
}

//implement the function using binary search
//if v is in array a[], whose length is n, return 1
//otherwise, return 0
int in_array(int a[], int n, int v) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        assert(mid >= 0 && mid < n);
        if (a[mid] == v) return 1;
        else if (a[mid] < v) left = mid + 1;
        else right = mid - 1;
    }
    return 0;
}

void run_search(int n, int m) {
    // NOTE: for some god forsaken reason, the outer function in `search.c` sometimes prints
    //       "n = %d, m = %d\n" TWICE!!! I HAVE NO IDEA WHY, AND IT ONLY HAPPENS IN THE
    //       GRADESCOPE TESTS. after around 20 minutes of painful debugging, I decided to
    //       just flush stdout and manually print it *once* myself. FML.
    fflush(stdout);
    // NOTE: it is entirely possible that i am just being a moron, however, and there's an
    //       obvious reason why this is not working, because somehow, printing it HERE
    //       (after stdout has been flushed), it STILL prints twice. so my current theory is
    //       that stdout doesnt get flushed until the function has already forked, and so the
    //       child process inherits the stdout buffer, and then prints it again later. but i
    //       have literally no idea how to actually fix this.
    // printf("n = %d, m = %d\n", n, m);
    
    int to_proc_a_pipe[2];
    int from_proc_a_pipe[2];
    
    if (pipe(to_proc_a_pipe) < 0) {
        perror("Error.");
        exit(-1);
    }
    if (pipe(from_proc_a_pipe) < 0) {
        perror("Error.");
        exit(-1);
    }
    
    pid_t pid_a = fork();
    if (pid_a == 0) { // process A
        //note this is process A
        
        // close file descriptors
        close(to_proc_a_pipe[PIPEFD_WRITE]);
        close(from_proc_a_pipe[PIPEFD_READ]);
        
        srand(3100);
        
        //now generate array A
        int A[n];
        A[0] = rand() % 10 + 1;
        for (int i = 1; i < n; i++) {
            A[i] = A[i-1] + rand() % 10 + 1; 
        }
        
        int v;
        int round = 0;
        while (read_int(to_proc_a_pipe[PIPEFD_READ], &v) > 0) {
            if (v != 1) break;
            assert(round < n);
            write_int(from_proc_a_pipe[PIPEFD_WRITE], A[round++]);
        }
        
        // close file descriptors
        close(to_proc_a_pipe[PIPEFD_READ]);
        close(from_proc_a_pipe[PIPEFD_WRITE]);
        
        exit(EXIT_SUCCESS);
    }
    
    // close file descriptors
    close(to_proc_a_pipe[PIPEFD_READ]);
    close(from_proc_a_pipe[PIPEFD_WRITE]);
    
    int to_proc_b_pipe[2];
    //pipe creation
    if (pipe(to_proc_b_pipe) < 0) {
        perror("Error.");
        exit(-1);
    }

    int from_proc_b_pipe[2];
    //pipe creation
    if (pipe(from_proc_b_pipe) < 0) {
        perror("Error.");
        exit(-1);
    }
    
    pid_t pid_b = fork();
    if (pid_b == 0) { // process B
        // not needed
        close(to_proc_a_pipe[PIPEFD_WRITE]);
        close(from_proc_a_pipe[PIPEFD_READ]);
        // close file descriptors
        close(to_proc_b_pipe[PIPEFD_WRITE]);
        close(from_proc_b_pipe[PIPEFD_READ]);
        
        srand(3504);
        
        //now generate array B
        int B[n];
        B[0] = rand() % 10 + 1;
        for(int i = 1; i < n; i++) {
            B[i] = B[i-1] + rand() % 10 + 1; 
        }
        
        int v;
        while (read_int(to_proc_b_pipe[PIPEFD_READ], &v) != 0) {
            if (v == -1) break;
            _Bool found = in_array(B, n, v);
            write_int(from_proc_b_pipe[PIPEFD_WRITE], found);
        }
        
        // close file descriptors
        close(to_proc_b_pipe[PIPEFD_READ]);
        close(from_proc_b_pipe[PIPEFD_WRITE]);
        
        exit(EXIT_SUCCESS);
    }
    
    close(to_proc_b_pipe[PIPEFD_READ]);
    close(from_proc_b_pipe[PIPEFD_WRITE]);
    
    // [see note above]
    // NOTE: the fact that this "fix" makes it print this line twice on my machine, and
    //       only once in the gradescope test vms is absolutely fucking cursed.
    printf("n = %d, m = %d\n", n, m);
    
    int v1, v2;
    int count = 0;
    for (int i = 0; i < n; i++) {
        write_int(to_proc_a_pipe[PIPEFD_WRITE], 1);
        read_int(from_proc_a_pipe[PIPEFD_READ], &v1);
        write_int(to_proc_b_pipe[PIPEFD_WRITE], m - v1);
        read_int(from_proc_b_pipe[PIPEFD_READ], &v2);
        
        if (v2) {
            printf("%d + %d = %d\n", v1, m - v1, m);
            count++;
        }
    }
    
    printf("There are %d pairs that add up to %d.\n", count, m);
    
    // send signal to children to die
    write_int(to_proc_a_pipe[PIPEFD_WRITE], -1);
    write_int(to_proc_b_pipe[PIPEFD_WRITE], -1);
    
    // wait for children to die
    waitpid(pid_a, NULL, 0);
    waitpid(pid_b, NULL, 0);
    
    // close file descriptors
    close(to_proc_a_pipe[PIPEFD_WRITE]);
    close(from_proc_a_pipe[PIPEFD_READ]);
    close(to_proc_b_pipe[PIPEFD_WRITE]);
    close(from_proc_b_pipe[PIPEFD_READ]);
}
