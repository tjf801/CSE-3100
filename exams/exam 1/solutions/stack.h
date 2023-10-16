#ifndef STACK_H
#define STACK_H

#include "linked_list.h"
typedef struct {
   node *top; // NULL means empty
} stack;

void push(stack *s, node *new_node);
node* pop(stack *s);
int empty(stack *s); // should be _Bool
void clear_stack(stack *s);

#endif // STACK_H