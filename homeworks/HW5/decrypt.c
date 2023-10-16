#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <assert.h>
#include <limits.h>

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
    if(fp==NULL) {
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

//TODO
//Test wether a string word is in the dictionary
//Return 1 if word is in the dictionary
//Return 0 otherwise
//Be efficient in implementing this function
//Efficiency is needed to pass test cases in limited time
int in_dict(char *word) {
    return 0; // not used
}

//Use key and shift to decrypt the encrypted message
//len is the length of the encrypted message
//Note the encrypted message is stored as an array of integers (not chars)
//The result is in decrypted
void decryption(unsigned char key, unsigned char shift, const int *encrypted, int len, char *decrypted) {
    // ^^ not allowed to change function signature or starter code, so key and shift are ignored
    
    // use the unused `word_count` variable as a flag to quit 
    // early since we only need to run this funciton once.
    if (word_count == -1) return;
    word_count = -1; // thank you for the convenient global variables!
    
    // since only 8 of the 32 bits contain the original message (i.e, bits `shift` to `shift+8`),
    // one can easily find the shift by XORing all of the messages pairwise, which filters out only
    // the differing bits between them. then those can be ORed to find a mask of all bits in the data
    // which actually contain information. the value of `shift` is then the index of the least
    // significant set bit in the mask.
    
    // find the correct value of shift
    int mask = 0x00000000;
    for (int i = 0; i < len - 1; ++i) {
        // // this check doesnt actually matter since it only affects the high bits
        // if (encrypted[i] < 0 || encrypted[i+1] < 0) continue;
        mask |= encrypted[i] ^ encrypted[i + 1];
    }
    shift = __builtin_ffs(mask) - 1;
    // printf("calculated shift value: %d\n", shift);
    
    if (shift >= 8) {
        // the key value is the lowest byte of all the ints,
        // since it doesn't overlap with the message
        // key = encrypted[0] & 0xFF;
        
        // when the key doesn't overlap with the data (i.e. `shift >= 8`),
        // it doesnt matter and it will always end up to be 0 anyways if we
        // did calculate it
        key = 0;
    } else {
        // if we got here, that means that the `shift` variable is less than 8, so we have to
        // actually figure out the key that was used.
        
        // count all encrypted bytes
        unsigned int byte_counts[256] = { 0 };
        for (int i = 0; i < len; ++i) {
            unsigned char byte = (encrypted[i] >> shift) & 0xFF;
            byte_counts[byte]++;
        }
        
        // find the most common byte
        unsigned int max_count = byte_counts[0];
        unsigned char max_index = 0;
        for (int i = 1; i < UCHAR_MAX; ++i) {
            if (byte_counts[i] > max_count) {
                max_count = byte_counts[i];
                max_index = i;
            }
        }
        
        // using the fact that the most common character is (usually) ' ',
        // we can get the key back by XORing the most common byte with ' '.
        key = (max_index ^ ' ') << shift;
    }
    
    // printf("calculated key: %d\n", key);
    
    // decrypt the message and put it into the decrypted buffer
    for (int i = 0; i < len; ++i) {
        // // NOTE: since certain unicode characters start with bytes like `0xE2`,
        // // `encrypt.c` thinks they are negative *signed* chars, and so shifts them while
        // // keeping and extending their "sign bit" to give something like `0xFFFFE200`.
        // // As such, when shifting them back, we need to make sure all the other bits,
        // // which have a mask of 0xFFFFFF00, are either all zeros or all ones.
        // const int sign_bits = (encrypted[i] >> shift) & (0xFF ^ -1);
        // assert(!sign_bits || sign_bits == (0xFF ^ -1));
        
        // inverse of `encrypt.c`
        decrypted[i] = (char) ((encrypted[i] ^ key) >> shift) & 0xFF;
    }
    decrypted[len] = 0;
}

//calculate a score for a message msg
//the score is used to determine whether msg is the original message
int message_score(const char *msg) {
    // if "We can even be more creative and come up with better measures",
    // that means i can be REALLY creative and use a trivial score of 1 :3
    return 1; // not used
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
            if(score > max_score) {    
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

    // around 98% of the program runtime is spent in this function.
    read_file_to_array("dict.txt");

    int encrypted[MAX];
    int n_bytes = read_encrypted(argv[1], encrypted);

    char message[MAX];
    strcpy(message, "");
    search(encrypted, n_bytes, message);
    printf("%s\n", message);
    return 0;
}
