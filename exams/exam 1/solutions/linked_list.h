#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct node_tag {
   char    v;				// data
   struct node_tag * next;	// A pointer to this type of struct
} node;						// Define a type. Easier to use.

node * create_node(char v);
void add_first(node **head, node *newnode);
node * remove_first(node **head);

#endif // LINKED_LIST_H