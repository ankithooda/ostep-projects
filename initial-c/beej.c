#include <stdio.h>

void increment(int *);
void double_a(int *, int);


void increment(int *c) {
	++*c;
}

void double_a(int *a, int len) {
	for (int i = 0; i < len; i++) {
		a[i] = a[i] * 2;
	}
}

int main() {
	printf("%zu \n", sizeof(5));
	printf("%zu \n", sizeof(4.55555533));
	printf("%zu \n", sizeof('C'));
	printf("%zu \n", sizeof(char));
	printf("%zu \n", sizeof(int)); 

	int i = 10;
	printf("Address of i is %p\n", (void *)&i);
	printf("Before inc %d\n", i);
	increment(&i);
	increment(&i);
	printf("After inc and inc %d\n", i);
		
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$444\n");

	int x[5] = {2, 5, 6};
	printf("Size of array %zu\n", sizeof(x));

	double_a(x, 3);
	
	for (int i = 0; i < 10; i++) {
		printf("x->%d\n", x[i]);
	}

	char *msg = "Hello World";
	msg[1] = 'S';
	printf("%s\n", msg);
}
