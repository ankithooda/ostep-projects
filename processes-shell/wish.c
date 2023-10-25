#include <stdio.h>
#include <string.h>
#include <limits.h>

int main(int argc, char *argv[]) {

    char input[LINE_MAX];
    char *token;
    char *saveptr;
    char *iter;

    printf("> ");
    fgets(input, LINE_MAX, stdin);
    printf(">> %s\n", input);
    printf(">>> %d\n", LINE_MAX);

    token = strtok_r(input, " ", &saveptr);
    printf("TOKEN - %s\n", token);
    while (token != NULL) {
        token = strtok_r(NULL, " ", &saveptr);
        printf("TOKEN - %s\n", token);
    }
    int i = 0;
    iter = saveptr;

    while (iter != NULL) {
        printf(">>>> %s\n", *(&iter + i));
        i = i + 1;
    }



    return 0;
}
