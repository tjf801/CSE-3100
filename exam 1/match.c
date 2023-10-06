#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

_Bool match(const char * const exp) {
    stack *s = (stack *)malloc(sizeof(stack));
    s->top = NULL;
    _Bool result = 1;
    
    for (int i = 0; result && i < strlen(exp); i++) { // why on god's green earth would you do this. strings are null terminated for a reason. jfc.
        if(exp[i] == '(' || exp[i] == '[' || exp[i]=='{') {
            push(s, create_node(exp[i]));
        }
        else if (exp[i] == ')' || exp[i] == ']' || exp[i] == '}') {
            node * n = pop(s);
            if (n == NULL) { // this exam has reminded me of why i hate c. thanks wei wei!
                result = 0;
                break;
            }
            switch (n->v) {
                case '(': {
                    if (exp[i] != ')') result = 0;
                    break;
                }
                case '[': {
                    if (exp[i] != ']') result = 0;
                    break;
                }
                case '{': {
                    if (exp[i] != '}') result = 0;
                    break;
                }
            }
            free(n);
        }
    }
    if (!empty(s)) {
        result = 0;
        clear_stack(s);
    }
    free(s);
    
    return result;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s expression\n", argv[0]);
        return -1;
    }
    char * exp = argv[1];
    
    printf("%s\n", exp);
    
    if (match(exp)) printf("match test passed.\n");
    else printf("match test failed.\n");
    
	return 0;
}