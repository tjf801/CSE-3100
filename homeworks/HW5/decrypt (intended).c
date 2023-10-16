#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <assert.h>

#define MAX 10240
#define MAX_WORD_COUNT 60000                //we have less than 60000 words
#define MAX_WORD_LENGTH 80                //each word is less than 80 letters

//Using these two global variables can be justified :)
char words[MAX_WORD_COUNT][MAX_WORD_LENGTH];        //2-d array to hold all the words
int word_count = 0;                    //number of words, initilized to 0

//Note the words in the dictionary file are sorted
//This fact could be useful
void read_file_to_array(char *filename) {
    FILE *fp;
    
    //open the file for reading
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Cannot open file %s.\n", filename);
        exit(-1);
    }
    
    //make sure when each word is saved in the array words,
    //it does not ends with a '\n'
    //see how this is done using fscanf 
    while(!feof(fp))
        fscanf(fp, "%s\n", words[word_count++]);
    
    fclose(fp);
}

//Test wether a string word is in the dictionary
//Return 1 if word is in the dictionary
//Return 0 otherwise
//Be efficient in implementing this function
//Efficiency is needed to pass test cases in limited time
int in_dict(char *word) {
    // // premature optimization: not needed
    // const static unsigned int letter_indices[27] = {
    //     0, 3479, 6689, 12181, 15957, 18545, 21102, 22938,
    //     24964, 27637, 28110, 28464, 30301, 33245, 34164, 
    //     35552, 40115, 40405, 44039, 50709, 53590, 55511, 
    //     56322, 57864, 57878, 58022, 58108
    // };
    // assert(word_count == 58108);
    // int low = letter_indices[(word[0] & 31)-1];
    // int high = letter_indices[word[0] & 31];
    
    int low = 0, high = word_count;
    
    while (low <= high) {
        int mid = (low + high) / 2;
        int cmp = strcmp(words[mid], word);
        
        if (cmp < 0) low = mid + 1;
        else if (cmp > 0) high = mid - 1;
        else return 1;
    }
    
    return 0;
}

//Use key and shift to decrypt the encrypted message
//len is the length of the encrypted message
//Note the encrypted message is stored as an array of integers (not chars)
//The result is in decrypted
void decryption(unsigned char key, unsigned char shift, const int *encrypted, int len, char *decrypted) {
    for (int i = 0; i < len; ++i)
        decrypted[i] = (char) ((encrypted[i] ^ key) >> shift) & 0xFF;
    decrypted[len] = 0;
}

//calculate a score for a message msg
//the score is used to determine whether msg is the original message
int message_score(const char *msg) {
    const char *start = msg;
    const char *end = NULL;
    char word_buffer[MAX_WORD_LENGTH];
    unsigned int num_words = 0;
    
    // checking every word is too slow, so only check the first 4
    for (int i = 0; i < 4; ++i, start = end) {
        // find the start and end of the next word (consecutive string of letters)
        while (*start && !isalpha(*start)) start++;
        end = start;
        while (*end && isalpha(*end)) end++;
        
        // stop if you reached the end
        if (!*end) break;
        
        // length of word
        const unsigned int n = end - start;
        
        // if its above MAX_WORD_LENGTH characters long, it cant be a word
        if (n > MAX_WORD_LENGTH) continue;
        
        // copy string and convert to lowercase
        for (int i = 0; i < n; ++i)
            word_buffer[i] = start[i] | 0b00100000;
        word_buffer[n] = 0;
        
        // check if the word is in the dict
        num_words += in_dict(word_buffer);
    }
    
    return num_words;
}

//search using all the (key, shift) combinations
//to find the original message
//result is saved in message
void search(const int *encrypted, int len, char *message) {
    char decrypted[MAX];

    int max_score = 0;
    strcpy(message, "");
    
    for(unsigned char k = 0; k < 255; k++) {
        for(unsigned char shift = 0; shift <= 24; shift++) {
            decryption(k, shift, encrypted, len, decrypted);
            int score = message_score(decrypted);
            if (score > max_score) {    
                max_score = score;
                strcpy(message, decrypted);
            }
        }
    }
}

//read the encrypted message from the file to encrypted
//return number of ~~bytes~~ integers read
int read_encrypted(const char *filename, int *encrypted) {
    int encrypted_fd = open(filename, O_RDONLY);
    ssize_t len = read(encrypted_fd, encrypted, MAX * sizeof(int)) / sizeof(int);
    close(encrypted_fd);
    return len;
}

//Do not change the main() function
int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("%s encrypted-message\n", argv[0]);
        return 0;
    }

    read_file_to_array("dict.txt");

    int encrypted[MAX];
    int len = read_encrypted(argv[1], encrypted);
    
    char message[MAX];
    strcpy(message, "");
    search(encrypted, len, message);
    printf("%s\n", message);
    return 0;
}
