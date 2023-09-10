#include <stdio.h>

int main() {
    double total = 0.0, x;
    
    for (unsigned int n = 1; scanf("%lf", &x) == 1; ++n) {
        total += x;
        printf("Total=%f Average=%f\n", total, total / n);
    }
}