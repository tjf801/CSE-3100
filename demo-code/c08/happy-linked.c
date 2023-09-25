//In this example, we use linked list to determine whether a positive integer is a happy number
//Given a positive integer, we repeatedly apply the following operation
//Summing up all the squared digits in the number, and the result is the next number
//For example, if the number is 19, then the next number is 1*1 + 9*9 = 82
//Then we apply the same operation on 82, we get 8*8 + 2*2 = 68,
//Then we will get 6*6 + 8*8 = 100
//Then 1*1 + 0*0 + 0*0 = 1
//For a positive integer, if we repeat the above operation and eventually reach 1,
//then the given number is a happy number.
//On the other hand, if at certain point, we reach a number that already seen before, then 
//the given number is not a happy number.

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//the next number
int next_number(int n)
{
	
	int sum = 0;
	while(n > 0)
	{
		sum += (n%10)*(n%10);
		n /= 10;
	}
	return sum;
}


typedef struct node_tag {
   int    v;				// data
   struct node_tag * next;	// A pointer to this type of struct
} node;						// Define a type. Easier to use.

//create a node with value v
node * create_node(int v) 
{
    node	* p = malloc(sizeof(node)); // Allocate memory
    assert(p != NULL);		// you can be nicer
    
    // Set the value in the node.
    p->v = v;		
    p->next = NULL;  
    return p;			// return
}

//add newnode the last of the linked list determined by *head and *tail
//note **head, **tail
void add_last(node **head, node **tail, node *newnode)
{
	if((*head) == NULL)
	{
		(*head) = (*tail) = newnode;
	}
	else
	{
		(*tail)->next = newnode;
		(*tail) = newnode;
	}
}

//remove the first node from the list
//note **head

node * remove_first(node **head) 
{
	node *p;

	p = (*head);
	if((*head)!=NULL) (*head) = (*head)->next;
	return p;
}

//wether v is in the list pointed by head
//this is useful for us to check we reach a repeated number
int in_list(node * head, int v)
{
	while(head!=NULL)
	{
		if(head->v == v) return 1;
		head = head->next;
	}
	return 0;
}

//print the list pointed by head
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

//function to determine whether m is happy
int happy(int m)
{
	node *head, *tail;
	head = tail = NULL;

	int n = m;
    while(!in_list(head, n))
	{
		node *p = create_node(n);
		add_last(&head, &tail, p);
        n = next_number(n);
	}
	if(n==1)
		printf("%d is a happy number: ", m);
	else
		printf("%d is not a happy number: ", m);
    print_list(head);
	//now we remove all the nodes in the list
	//and free all the allocated memory
	node *p; 
	while(head !=NULL)
	{
		p = remove_first(&head);
		if(p!=NULL) free(p);
	}	
	return n==1;
}
int main()
{
	//try all the positive integer no more than 1000
	for(int k = 1; k<=1000; k++)
		happy(k);
	return 0;
}

