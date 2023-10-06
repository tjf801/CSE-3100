#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//TODO
void letter_counts(const size_t n, const char a[static n], unsigned int counts[26]) {
    for (size_t i = 0; i < n; ++i) counts[a[i]-'a']++;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s n\n", argv[0]);
        return -1;
    }
    int n = atoi(argv[1]);
    printf("n=%d\n", n);
    assert(n >= 1 && n <= 10000);

    char a[n];
    int sum = 0;
    for (int i=0; i<n; i++) {
        sum += 2*i +1;
        a[i] = sum % 26 + 97;
    }
    
    // there was literally a bug here where `counts` was filled with garbage
    // values because the starter code didnt initialize this array.
    // i guess c programmers are called "C-nile" for a reason.
    unsigned int counts[26] = { 0 };
    letter_counts(n, a, counts);
    
    for (int i = 0; i < 26; ++i)
        if (counts[i]) printf("%c %d\n", i+'a', counts[i]);
    
    return 0;
}