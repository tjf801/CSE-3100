#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define MAX 10000

typedef struct node_tag
{
   int    v;                    // data
   struct node_tag * next;  // A pointer to this type of struct
} node;                        // Define a type. Easier to use.


node * create_node(int v)
{
    node    * p = malloc(sizeof(node)); // Allocate memory
    assert(p != NULL);      // you can be nicer

    // Set the value in the node.
    p->v = v;
    p->next = NULL;
    return p;           // return
}

void print_list(node *head)
{   
    while(head!=NULL)
    {   
        printf("%d ", head->v);
        head = head->next;
    }
    printf("\n");
}

void add_first(node **head, node *newnode)
{
    if(*head == NULL)
    {
        *head = newnode;
        newnode->next = NULL;
    }
    else
    {
        newnode->next = *head;
        *head = newnode;
    }
}

node * remove_first(node **head) 
{
	node *p;

	p = (*head);
	if((*head)!=NULL) (*head) = (*head)->next;
	return p;
}

void remove_multiple(node **head, int k)
{
	//printf("the list:\n");
	print_list(*head);
	printf("Remove multiples of %d\n", k);
	node *p = *head;
	if(p == NULL) return;

	if(p->v % k == 0 && p->v !=k)
	{
		p = remove_first(head);
		free(p);
		remove_multiple(head, k);
		return;
	}
	while(p != NULL && p->next != NULL)
	{
		node *q = p->next;
		if(q->v % k == 0 && q->v != k)
		{
			p->next = q->next;
			free(q);
		}
		else p = p->next;
	}
}

void free_all(node **head)
{
	while(*head !=NULL)
	{
		node *p = remove_first(head);
		free(p);
	}
}
int list_length(node *head)
{
	int count = 0;
	while(head !=NULL)
	{
		head = head->next;
		count ++;
	}
	return count;
}
//Do not change the main function
int main(int argc, char *argv[])
{
	if(argc!=2)
	{
		printf("Usage: %s n\n", argv[0]);
		return -1;
	}
	int n = atoi(argv[1]);
	assert(n >= 1 && n <= MAX);
	node *head, *p;
	head = NULL;

	int i;
	
	for(i=n; i>=2; i--)
	{
		p = create_node(i);
		add_first(&head, p);
	}

	for(i = 2; i<n; i++)
	{
		remove_multiple(&head, i);
	}
	
	printf("%d primes between 1 and %d:\n", list_length(head), n);	
	print_list(head);
	free_all(&head);
	printf("%d items left in the list after free_all().\n", list_length(head));
	return 0;
}
