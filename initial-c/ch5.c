/*
    Refresher of Chapter 5 K&R C.
*/
#include <stdio.h>
#include <string.h>

char* my_strtok_r(char* , const char*, char **);

int main() {
    char *token;
    char *saveptr;

    // char m[] = "command a b c";
    // printf("%s\n", my_strtok_r(m, " ", &saveptr));

    // strtok_r
    char s[] = "command a b c";

    token = my_strtok_r(s, " ", &saveptr);
    printf("STRTOK_R %s\n", token);
    while (token != NULL) {
        token = my_strtok_r(NULL, " ", &saveptr);
        printf("STRTOK_R %s\n", token);

    }

    return 0;
}

char *my_strtok_r(char *s, const char *delim, char **saveptr) {


    if (s == NULL) {
        s = *saveptr;
    }
    if (*s == '\0') {
        return NULL;
    }
    int i = 0;
    while (*(s+i) != *delim) {
        i = i + 1;
    }
    *(s+i) = '\0';
    *saveptr = (s + i + 1);
    return s;
}
