#include <stdio.h>

int main() {
    int x = 3490;
    int *p, **q;
    
    p = &x;
    q = &p;

    printf("%p-%d\n", (void *)q, **q);
    printf("%p-%d\n", (void *)p, *p);

    return 0;
}