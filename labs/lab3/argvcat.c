#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* print out an error message and exit */
void my_error(char *s) {
    perror(s);
    exit(1);
}

/* Concatnate two strings.
 * Dynamically allocate space for the result.
 * Return the address of the result.
 */
char *my_strcat(const char * const s1, const char * const s2) {
    // i love O(n+m) time complexity!!! thank you C <3
    const size_t s1_len = strlen(s1), s2_len = strlen(s2);
    char * const result = calloc(s1_len + s2_len + 1, sizeof(char));
    strncpy(result, s1, s1_len);
    strncat(result, s2, s2_len);
    return result;
}

int main(int argc, char *argv[]) {
    // // teehee :3
    // for (int i = 0; i < argc; ++i) printf("%s", argv[i]);
    // printf("\n");
    
    char *s = my_strcat("", argv[0]);
    
    for (int i = 1; i < argc; i ++) {
        char *tmp = my_strcat(s, argv[i]);
        free(s);
        s = tmp;
    }
    
    printf("%s\n", s);
    
    free(s);
    
    return 0;
}
