#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

int match(char *exp)
{
    stack *s = (stack *)malloc(sizeof(stack));
    s->top = NULL;
    int result = 1;

    for(int i = 0; i < strlen(exp); i++)
    {
        if(exp[i] == '(' || exp[i] == '[' || exp[i]=='{')
        {
            //TODO

        }
        else if(exp[i] == ')' || exp[i] == ']' || exp[i] == '}')
        {
            //TODO

        }
    }
    if(!empty(s)) 
    {
        //TODO
    }
    free(s);
    return result;
}

int main(int argc, char *argv[])
{   
    if(argc != 2) 
    {
        printf("Usage: %s expression\n", argv[0]);
        return -1;
    }
    char * exp = argv[1];

    printf("%s\n", exp);
    if(match(exp))
    {
        printf("match test passed.\n");
    }
    else
    {
        printf("match test failed.\n");
    }
	return 0;
}