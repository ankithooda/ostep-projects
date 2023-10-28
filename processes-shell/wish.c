#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#define INPUT_LEN 1000

void parse_input(char *, char *[]);
void process(char *[]);

int main(int argc, char *argv[]) {

    char *input;                // input buffer
    char *command[INPUT_LEN];   // Tokenized input
    FILE *stream;
    size_t len = 0;
    
    if (argc == 2) {
        stream = fopen(argv[1], "r+");
    } else {
        stream = stdin;
    }
    while (1) {
        // Print prompt only in interactive mode.
        if (argc == 1) { printf("> "); }
        
        if (getline(&input, &len, stream) != -1) {
            parse_input(input, command);
            // int i = 0;
            // while (command[i] != NULL) {
            //     printf("%s\n", command[i]);
            //     i = i + 1;
            // }
            process(command);
        } else {
            free(input);
            exit(EXIT_SUCCESS);
        }
    }
    return 0;
}

void parse_input(char *input, char *command[]) {
    int i = 1;
    char *saveptr;

    // Remove trailing newline
    input[strcspn(input, "\n")] = '\0';

    // Parse input into command and arguments.
    command[0] = strtok_r(input, " ", &saveptr);

    while (command[i-1] != NULL) {
        command[i] = strtok_r(NULL, " ", &saveptr);
        i = i + 1;
    }
}

void process(char *command[]) {

    // Special handling of exit command.
    if (strcmp(command[0], "exit") == 0) {
        _exit(EXIT_SUCCESS);
    }
    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "Could spawn process.");
    } else if (rc == 0) {
        int exec_code = execvp(command[0], command);
        if (exec_code < 0) {
            fprintf(stderr, "An error has occurred\n");
            _exit(EXIT_FAILURE);
        }
    } else {
        wait(NULL);
    }
}