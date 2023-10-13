//execvp_demo.c 

#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
int main() 
{ 
        //A null terminated array of character 
        //pointers 
        char *args[]={"./exec",NULL}; 
        execvp(args[0],args); 
    
        /*All statements are ignored after execvp() call as this whole 
        process(execvp_demo.c) is replaced by another process (exec.c) 
        */
        printf("Ending-----\n"); 
    
    return 0; 
} 

