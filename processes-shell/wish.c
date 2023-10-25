#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[]) {

    char input[LINE_MAX]; // input buffer
    char *command[100];   // Tokenized input
    char *saveptr;        // Internal pointer for strtok_r
    int i = 1;            // iterator variable
    while (1) {
        // Prompt
        printf("> ");
        fgets(input, LINE_MAX, stdin);

        // Parse input into command and arguments.
        command[0] = strtok_r(input, " ", &saveptr);    
        while (command[i-1] != NULL) {
            command[i] = strtok_r(NULL, " ", &saveptr);
            i = i + 1;
        }
        // Replace delimiter with NULL
        command[i - 1] = NULL;

        for (i = i - 1; i < 100; i++) {
            command[i] = NULL;
        }

        for (i = 0; command[i] != NULL; i++) {
            printf("TOKEN %s-%ld\n", command[i], strlen(command[i]));
        }

        // Create new process for the command.
        int rc = fork();

        if (rc < 0) {
            fprintf(stderr, "Unable to create process for running the command.\n");
        } else if (rc == 0) {
            int exec_code = execvp(command[0], command);
            if (exec_code < 0) {
                fprintf(stderr, "EXEC_CODE %d\nERRNO %d\n", exec_code, errno);
                fprintf(stderr, "Unable to run command.\n");
                _exit(1);
            }
        } else {
            // Wait for child process to terminate.
            wait(NULL);
        }
    }

    return 0;
}
