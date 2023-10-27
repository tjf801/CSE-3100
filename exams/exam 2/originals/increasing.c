#include <stdio.h>
#include <stdlib.h>
#include "redirect.h"

#define MAX 100

int main(int argc, char *argv[])
{

	char word[MAX];
	if(argc!=3)
	{
		printf("%s filename file_result\n", argv[0]);
		return 0;
	}

	redirectStdin(argv[1]);
	redirectStdout(argv[2]);

	while(scanf("%s\n", word)!=EOF)
        if(increasing(word)) printf("%s\n", word);
	return 0;
}
