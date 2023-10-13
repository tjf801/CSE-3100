//execv_demo.c 

#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
int main() 
{ 
        //A null terminated array of character 
        //pointers 
        char *args[]={"./exec",NULL}; 
        execv(args[0],args); 
    
        /*All statements are ignored after execv() call as this whole 
        process(execv_demo.c) is replaced by another process (exec.c) 
        */
        printf("Ending-----"); 
    
    return 0; 
} 

