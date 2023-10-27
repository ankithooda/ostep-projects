#include <stdio.h>

void swap(int*, int*);

int main() {
    int a = 5;
    int b = 8;

    printf("%d - %d\n", a, b);
    swap(&a, &b);
    printf("%d - %d\n", a, b);
    return 0;
}

void swap(int *a, int *b) {
    int t;

    t = *a;
    *a = *b;
    *b = t;
}