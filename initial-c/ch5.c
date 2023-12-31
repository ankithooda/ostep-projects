/*
    Refresher of Chapter 5 K&R C.
*/
#include <stdio.h>
#include <string.h>

char* my_strtok_r(char* , const char, char **);

int main() {
    char *token;
    char *saveptr;

    char s[] = "Xasa aas asassa sasa";

    token = strtok_r(s, "s", &saveptr);
    printf("STRTOK_R %s\n", token);
    while (token != NULL) {
        token = strtok_r(NULL, "s", &saveptr);
        printf("STRTOK_R %s\n", token);
        printf("SAVEPTR %s\n", (*saveptr));

    }

    return 0;
}

char *my_strtok_r(char *s, const char delim, char **saveptr) {
    
    // For the subsequent calls we use the address stored in *saveptr.
    // Otherwise we reset *saveptr to NULL.

    if (s == NULL) {
        s = *saveptr;
    } else {
        *saveptr = NULL;
    }

    // Empty string returns NULL
    if (*s == '\0') {
        return NULL;
    }

    int i = 0;
    while (*(s + i) != '\0') {
        if (*(s + i) == delim) {
            *(s + i) = '\0';
            break; 
        }
        i = i + 1;
    }
    // Store address in saveptr before returning.
    *saveptr = (s + i + 1);
    return s;
}
