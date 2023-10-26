#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include <errno.h>

#define INPUT_LEN 100

int main(int argc, char *argv[]) {

    char input[LINE_MAX];       // input buffer
    char *command[INPUT_LEN];   // Tokenized input
    char *saveptr;              // Internal pointer for strtok_r
    int i;                      // iterator variable
    while (1) {
        // Prompt user input.
        printf("> ");
        fgets(input, LINE_MAX, stdin);

        // Remove trailing newline
        input[strcspn(input, "\n")] = '\0';

        // Parse input into command and arguments.
        command[0] = strtok_r(input, " ", &saveptr);
        i = 1;  
        while (command[i-1] != NULL) {
            command[i] = strtok_r(NULL, " ", &saveptr);
            i = i + 1;
        }

        // Create new process for the command.
        int rc = fork();

        if (rc < 0) {
            fprintf(stderr, "Unable to create process for running the command.\n");
        } else if (rc == 0) {
            int exec_code = execvp(command[0], command);
            if (exec_code < 0) {
                fprintf(stderr, "Unable to run command.\n");
                _exit(EXIT_FAILURE);
            }
        } else {
            // Wait for child process to terminate.
            wait(NULL);
        }

        // Empty buffer
        for (i = 0; i < INPUT_LEN; i++) {
            command[i] = NULL;
        }
    }
    return 0;
}
