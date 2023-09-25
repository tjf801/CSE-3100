//In this example, we explore the function pointers
//and how to use function pointers for qsort().
//This might be one of the most useful examples if you need to
//use qsort() in C.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int stringCompare(const void* s1,const void* s2)
{
   return strcmp(*(char**)s1,*(char**)s2);
}


int int_cmp(const void *a, const void *b)
{
	if(*(int *)a < *(int *)b) return -1;
	else if(*(int *)a == *(int *)b) return 0;
	else return 1;
}

int main()
{
	int a[10];

    	printf("a[] before qsort:\n");
	for(int i=0; i<10; i++)
	{
		a[i] = 10 - i;
		printf("%d ", a[i]);
	}
	printf("\n");

	printf("a[] after qsort:\n");	
	qsort(a, 10, sizeof(int), int_cmp);
	for(int i=0; i<10; i++)
	{
		printf("%d ", a[i]);
	}
	printf("\n");

	char** words = (char**)malloc(sizeof(char*)*5);

	//It helps to think words as an array of pointers
	//It can make your thinking and coding much easier

	words[0] = "z";
	words[1]  = "zz";
	words[2] = "zzz";
	words[3] = "yy";
	words[4] = "y";
	printf("words before qsort:\n");
	for(int i= 0; i<5; i++)
	{
		printf("%s\n", words[i]);
	}

	//Note the name of a function can be used as the address of the function
	//In other words, it is a pointer pointing to the function 
	qsort(words, 5, sizeof(char *), stringCompare);

	printf("words after qsort:\n");
	for(int i= 0; i<5; i++)
	        printf("%s\n", words[i]);

	free(words);
}
