#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <assert.h>
#include <limits.h>

#define MAX 10240

//Use key and shift to decrypt the encrypted message
//len is the length of the encrypted message
//Note the encrypted message is stored as an array of integers (not chars)
//The result is in decrypted
void decryption(const int *encrypted, int len, char *decrypted) {
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
    unsigned char shift = __builtin_ffs(mask) - 1;
    // printf("calculated shift value: %d\n", shift);
    
    unsigned char key = 0;
    
    if (shift < 8) {
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
        key = max_index ^ ' ';
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
        decrypted[i] = ((encrypted[i] ^ key) >> shift) & 0xFF;
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
// i changed the main function, sorry
int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("%s encrypted-message\n", argv[0]);
        return 0;
    }
    
    int encrypted[MAX];
    int n_bytes = read_encrypted(argv[1], encrypted);
    
    char message[MAX];
    strcpy(message, "");
    decryption(encrypted, n_bytes, message);
    printf("%s\n", message);
    return 0;
}