#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#define INPUT_LEN 100

int main(int argc, char *argv[]) {

    char input[LINE_MAX];       // input buffer
    char *command[INPUT_LEN];   // Tokenized input
    char *saveptr;              // Internal pointer for strtok_r
    int i;                      // iterator variable
    FILE *input_file;
    
    if (argc == 2) {
        input_file = fopen(argv[1], "r+");
    } else {
        input_file = stdin;
    }
    while (1) {
        // Prompt user input.
        // Empty buffer
        for (i = 0; i < LINE_MAX; i++) {
            input[i] = '\0';
        }
        for (i = 0; i < INPUT_LEN; i++) {
            command[i] = NULL;
        }
        printf("> ");
        fgets(input, LINE_MAX, input_file);

        // Remove trailing newline
        input[strcspn(input, "\n")] = '\0';

        // Parse input into command and arguments.
        command[0] = strtok_r(input, " ", &saveptr);
        i = 1;  
        while (command[i-1] != NULL) {
            command[i] = strtok_r(NULL, " ", &saveptr);
            i = i + 1;
        }

        // Run if some command has been parsed.
        if (command[0] != NULL) {

            // Create new process for the command.
            int rc = fork();

            

            if (rc < 0) {
                fprintf(stderr, "Unable to create process for running the command.\n");
            } else if (rc == 0) {
                int exec_code = execvp(command[0], command);
                if (exec_code < 0) {
                    fprintf(stderr, "%d\n%d\n", exec_code, errno);
                    fprintf(stderr, "An error has occurred\n");
                    _exit(EXIT_FAILURE);
                }
            } else {
                // Wait for child process to terminate.
                wait(NULL);
            }
        }
    }
    return 0;
}
