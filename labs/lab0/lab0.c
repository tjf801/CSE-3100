#include <stdio.h>

int main(void) {
    printf("Hello, World!\n");
    
    // Use a while loop to calculate the sum of positive even integers below 200 and print the result to the standard output.
    int sum = 0, i = 0;
    while (i < 200) {
        sum += i;
        i += 2;
    }
    printf("%d\n", sum);
    
    return 0;
}
