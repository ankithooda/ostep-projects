#include <stdio.h>
#include <stdlib.h>

char *readline(FILE *fp) {
    int offset = 0;
    int bufsize = 4;
    char *buffer;
    char c;

    buffer = malloc(bufsize);

    if (buffer == NULL) {
        return NULL;
    }

    while (c == fgetc(fp), c != '\n' && c != EOF) {
        
    }
}