#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct node_tag {
	struct node_tag * prev;		// A pointer to this type of struct
	int    v;			// data
	struct node_tag * next;		// A pointer to this type of struct
} node;

node * create_node(int v) 
{
    node * p = malloc(sizeof(node)); // Allocate memory
    assert(p != NULL);          // you can be nicer
    
    // Set the value in the node.
    p->prev = NULL;
    p->v = v;           
    p->next = NULL;  
    return p;                   // return
}

void add_last(node **head, node **tail, node *newnode)
{
        if((*head) == NULL)
                (*head) = (*tail) = newnode;
        else
        {
                (*tail)->next = newnode;
		newnode->prev = (*tail);
                (*tail) = newnode;
        }
}

node * remove_first(node **head, node **tail) 
{
        node *p = (*head);
        if((*head)!=NULL)
	{
		if((*head)->next!=NULL)
			(*head)->next->prev = NULL;
		else (*tail) = NULL;
		(*head) = (*head)->next;
	}
        return p;
}

void print_list(node *head)
{
        node *p = head;

        while(p!=NULL)
        {
                printf("%d ", p->v);
                p = p->next;
        }
        printf("\n");
}
//TODO
void reverse_list(node **head, node **tail)
{
	//fill in your code
}
//TODO
void free_all(node **head, node **tail)
{
	//fill in your code
}

//Do not change the main function
int main(int argc, char * argv[])
{
	assert(argc == 2);
	int n = atoi(argv[1]);
	
	node *p, *head=NULL, *tail=NULL;

	for(int i=0; i<n; i++)
	{
		p = create_node(i);
		add_last(&head, &tail, p);	
	}		
	print_list(head);
	reverse_list(&head, &tail);
        print_list(head);
	free_all(&head, &tail);
	return 0;	
}
