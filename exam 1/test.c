#include <stdio.h>
#include <stdlib.h>

int main() {
    int a[] = {1};
    
    int *p = a;
    *p++;
    printf("%d\n", a[0]);
    return 0;
}