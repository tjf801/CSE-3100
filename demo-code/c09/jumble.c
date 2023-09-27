//In this example, we learn how to handle file and practice some string operations.
//We write a game called jumble for this purpose.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#define MAX 70000
#define MAX_LINE 80
//remove \n \r from a string
void remove_newline(char word[])
{
	int i = 0;
	while(word[i])
	{	
		if(word[i] == '\n') word[i] = '\0';
		if(word[i] == '\r') word[i] = '\0';
		i++;
	}
}

//test whether a string only contains alphabetical letters
int isalpha_string(char word[])
{
	for(int i=0; i<strlen(word); i++)
		if(!isalpha(word[i])) return 0;
	return 1;
}

//randomly choose a number between 0 and count -1
int choose_k(int count)
{
	return rand() % count;
}

//scramble a word so that letters appear in an arbitrary order
//Note the usage of const
void scramble(const char *word, char *scrambled)
{
	strcpy(scrambled, word);
	for(int i=strlen(word); i>=1; i--)
	{
		int k = rand() % i;
		char ch = scrambled[k];
		scrambled[k] = scrambled[i-1];
		scrambled[i-1] = ch;
	}
}

int main()
{

	char words[MAX][MAX_LINE];
	FILE *fp;

	//open the file for reading
	fp = fopen("dict.txt", "r");
	if(fp==NULL)
	{
		printf("Cannot open the file.\n");
		return -1;
	}
	int count = 0;
	while(fgets(words[count], MAX_LINE, fp))
	{
		//only add words that only contail alphabetical letters to the array words
		remove_newline(words[count]);
		if(isalpha_string(words[count])) count ++;
	}
	printf("%d alphabetical words.\n", count);
	//close the file
	fclose(fp);

	srand(time(NULL));
	char answer[MAX_LINE];
	int max_score, score;
	max_score = score = 3;
	int length = 3;
	do
	{
		char *word;
		do
		{
			word = words[choose_k(count)];
		}while(strlen(word) > length);
		//only use words that have string length <= length
		char scrambled[MAX_LINE];
		scramble(word, scrambled);
		printf("%s\n", scrambled);
		scanf("%s", answer);
		if(!strcmp(answer, word))
		{
			printf("Correct answer!\n");
			score ++;
            		max_score = max_score < score ? score : max_score;
			printf("Current score: %d, max score: %d\n", score, max_score);
		}
		else
		{
			printf("Wrong answer! The word is %s\n", word);
			printf("Current score: %d, max score: %d\n", --score, max_score);
		}
		//ajust length based on user's performance
		length = score > 3? score : 3;
	}while(score > 0);
	return 0;
	//You should see that the design of the game has a flaw.
	//If the scrabmled word is 'on' and you type 'on' as
	//the answer, the computer might say your answer is wrong,
	//and the correct answer is 'no'.
	//Try to fix this flaw.
}
