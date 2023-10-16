#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

void push(stack *s, node *new_node) {
    add_first(&s->top, new_node);
}

node* pop(stack *s) {
    return remove_first(&s->top);
}

int empty(stack *s) { // should return _Bool
    return s->top == NULL;
}

void clear_stack(stack *s) {
    while (s->top != NULL) {
        node * n = remove_first(&s->top);
        if (n != NULL) // probably unnecessary, but whatever
            free(n); // this just feels so *wrong*. what if n was stack memory? or allocated with a different allocator than std `malloc`? or someone else has a reference to the top node? or god forbid another thread had a reference to it? or ...
    }
}