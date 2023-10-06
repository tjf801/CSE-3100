#ifndef STACK_H
#define STACK_H

#include "linked_list.h"
typedef struct {
   node *top;
} stack;		

void push(stack *s, node *new_node);
node* pop(stack *s);
int empty(stack *s);
void clear_stack(stack *s);

#endif // STACK_H