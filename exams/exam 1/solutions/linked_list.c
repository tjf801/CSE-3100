#include "linked_list.h"

//create a node with value v
node * create_node(char v) 
{
    node	* p = malloc(sizeof(node)); // Allocate memory
    assert(p != NULL);		// you can be nicer
    
    // Set the value in the node. 
    p->v = v;		
    p->next = NULL;  
    return p;			// return
}


//add_first() 
//note **head

void add_first(node **head, node *newnode)
{
    if((*head) == NULL)
    {
        (*head) = newnode;
    }
    else
    {
        newnode->next = (*head);
        (*head) = newnode;
    }
}


//remove the first node from the list
//note **head
//return a pointer to the removed content

node * remove_first(node **head) 
{
        node *p;

        p = (*head);
        if((*head)!=NULL) (*head) = (*head)->next;
        return p;
}
