#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_WORD_COUNT 60000                //we have less than 60000 words
#define MAX_WORD_LENGTH 80                //each word is less than 80 letters

char words[MAX_WORD_COUNT][MAX_WORD_LENGTH];        //2-d array to hold all the words
int count = 0;                    //number of words, initilized to 0


//read words from the file to the array words declared above
//also update the number of words (update variable count)
void read_file_to_array(char *filename)
{
    FILE *fp;

    //open the file for reading
    fp = fopen(filename, "r");
    if(fp==NULL)
    {
        printf("Cannot open the file.\n");
        exit(-1);
    }
    //make sure when each word is saved in the array words,
    //it does not ends with a '\n'
	//see how this is done using fscanf 
    while(!feof(fp))
    {
        fscanf(fp, "%s\n", words[count]);
        count ++;
    }
    fclose(fp);    
}
//test wether a word is in the array words
int is_word(char *word)
{
    for(int i = 0; i< count; i++)
    {
        if(!strcmp(words[i], word)) return 1;
    }
    return 0;
}

//test wether a word is a compound word
int compound_test(char * word)
{

	char left[MAX_WORD_LENGTH], right[MAX_WORD_LENGTH];    

	for(int k = 0; k< strlen(word) - 1; k++)
	{
		//note how we add a '\0' at the end to make left a string
		strncpy(left, word, k +1);
		left[k +1] = '\0';
    		strcpy(right, word + k + 1);

    		//if both left and right are words, print the result as below    
    		if(is_word(left) && is_word(right))
    		{
        		printf("%s is a compound word.\n%s = %s + %s\n", word, word, left, right);
			return 1;
    		}
	}
	return 0;
}

int main(int argc, char* argv[]) 
{
    read_file_to_array("dict.txt");

	//open a file for write
	FILE *fp = fopen("compound-list.txt", "w");
	if(fp == NULL)
	{
		perror("Cannot open the file.\n");
		exit(-1);
	}
	for(int i = 0; i < count; i++)
        	if(compound_test(words[i])) fprintf(fp, "%s\n", words[i]);
	fclose(fp);
	return 0;	
}

