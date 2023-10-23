//In this assignment, we practice using pipe. We will write to a pipe and read from a pipe.

//In this assignment, we use fork() to create multiple processes to solve a puzzle.
//The puzzle is specified by the following array
//int a[] = {3, 6, 4, 1, 3, 4, 2, 5, 3, 0};
//A walker walks along the index of this array.
//He starts at the index 0, where a[0] is 3, which indicates that
//the walker can move 3 steps. If the walker move to the left, he would be at index
//-3, which is out of the range. Hence, he can only move to the right, after he makes the move,//he will be at index 3. Since a[3] = 1, he can move to the left or right by one move.

//Note whenever he makes his move, he should be in the range [0, 9]. He is not allowd to move 
//outside of this range.

//The goal of the puzzle is to for the walker to reach index 9, where a[9] = 0. And he will
//stop there.

//What we need to do is to find the solutions of this puzzle. We limit the solutions to have
//at most 10 moves.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h> 

#define MAX_LINE 80
#define MAX 100
void write_solution(int b[], int moves, int pd)
{
    char buffer[MAX_LINE], temp_str[MAX_LINE];

    strcpy(buffer, "");
    for(int k = 0; k < moves; k++)
    {
        sprintf(temp_str, "->%d ", b[k]);
        strcat(buffer, temp_str);
    }
    strcat(buffer, "\n");
    //Add one line of code to write the buffer to the pipe pd

}

int read_solution(int pd, char buffer[])
{
    char c;
    int count = 0;
    //Note how we read from the pipe
    while(read(pd, &c , 1) > 0)
    {
        if(c != '\n')
        {
            buffer[count] = c;
            count ++;
        }
        else
        {
            buffer[count] = '\0';
            return 1;
        }
    }
    if(count > 0)
    {
        buffer[count] = '\0';    
        return 1;
    }
    return 0;    
}

int main(int argc, char *argv[])
{
    //We use the array a to describe the puzzle
    int a[] = {3, 6, 4, 1, 3, 4, 2, 5, 3, 0};
    //We use the array b to save the moves the walker makes
    //Essentially, we save each index that the walker reaches in array b
    //For example, b[0] = 3 since the walker goes to index 3 for the first move
    int b[10];    
    int cur = 0;
    int moves = 0;

    int n = 10;

    int pd[2];
	//pipe creation
    if(pipe(pd) == -1)
    {
        perror("Error.");
        return -1;
    }

    pid_t pid;
    pid = fork();
    if(pid == 0)
    {
        //We close pd[0] since child process do not need to read from the pipe
        close(pd[0]);
        for(int i = 0; i< n; i++)
        {
            pid_t cpid = fork();
            if(cpid == 0)
            {   
                //If we find a solution, we write the solution to the pipe
                if(a[cur]==0)
                {
                    b[moves - 1] = cur;
                    write_solution(b, moves, pd[1]);
                    //It is crucial to close pd[1] here
                    close(pd[1]);
                    return 0;
                }
                else if(cur + a[cur] >= 0 && cur + a[cur] <n)
                {
                    //Add your code here



                }
            }
            else
            {
                int status;
				//wait for the child process to finish
                waitpid(cpid, &status, 0);
                //If we find a solution, we write the solution to the pipe
                if(a[cur]==0)
                {
                    b[moves - 1] = cur;
                    write_solution(b, moves, pd[1]);
                    //It is crucial to close pd[1] here
                    close(pd[1]);
                    return 0;
                }
                else if(cur - a[cur] >= 0 && cur - a[cur] <n)
                {
                    //Add your code here



                }
            }
        }
        //It is crucial to close pd[1] here
        close(pd[1]);
        //The following return statement is crucial, why?
        return 0;
    }
    //Only the parent process will run the following code
    //There is one parent process here

    int status;
	//wait for the child process to finish
    waitpid(pid, &status, 0); 
    //It is crucial to close pd[1] here.
    close(pd[1]);
    char buffer[MAX_LINE];
    int count = 0;
    char results[MAX][MAX_LINE];

    //Next we read results from the pipe and write them into the array results
	//All the previous close(pd[1]) statements are neccesary. Otherwise, 
	//read_solution() will not finish since we can never reach EOF for this pipe.
    while(read_solution(pd[0], buffer))
    {
        strcpy(results[count++], buffer);
    }
    printf("Found solutions %d times.\n", count);    
    //Next we find the shortest solution
    int min_len = strlen(results[0]);
    int min_k = 0;
    for(int i = 1; i< count; i++)
    {
        if(strlen(results[i]) < strlen(results[min_k]))
        {
            min_len = strlen(results[i]);
            min_k = i;
        }
    }
    //Print the shortest solution
    printf("%d moves in the shortest solution.\n", min_len/4);
    printf("Shortest solution:\n%s\n", results[min_k]);
    close(pd[0]);
    return 0;
}
