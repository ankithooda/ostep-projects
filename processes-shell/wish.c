#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>


// TODO: Revise this arbitrary limits.
#define INPUT_LEN 1000
#define PATH_LEN 100
#define PATHNAME_LEN 150

// Static variable for storing path.
char path[PATH_LEN];

// Struct representing the parsed command.
struct parsed_data {
    char *command[INPUT_LEN];
    short redirect;
    char *redirect_file;
};

void parse_input(char *, char *[], struct parsed_data *);
void process(char *[], struct parsed_data *);

int main(int argc, char *argv[]) {

    char *input;                // input buffer
    char *command[INPUT_LEN];   // Tokenized input
    FILE *stream;               // File object to represent input file or stdin.
    size_t len = 0;

    struct parsed_data pd;

    // Intialize path to the bin directory.
    strcpy(path, "/bin");


    // Set appropriate value for the input stream.
    if (argc == 2) {
        stream = fopen(argv[1], "r+");
    } else {
        stream = stdin;
    }
    while (1) {
        // Print prompt only in interactive mode.
        if (argc == 1) { printf("> "); }
        
        if (getline(&input, &len, stream) != -1) {
            if (strlen(input) > 1) {
                parse_input(input, command, &pd);
                // int i = 0;
                // while (command[i] != NULL) {
                //     printf("%s\n", command[i]);
                //     i = i + 1;
                // }
                process(command, &pd);
            }
        } else {
            free(input);
            exit(EXIT_SUCCESS);
        }
    }
    // TODO: Handle SIGINT to free memory.
    return 0;
}

void parse_input(char *input, char *command[], struct parsed_data *pd) {


    int i = 1;

    char *saveptr;

    // Remove trailing newline
    input[strcspn(input, "\n")] = '\0';

    // Parse input for redirection.
    input = strtok_r(input, ">", &saveptr);

    pd->redirect_file = strtok_r(NULL, ">", &saveptr);

    if (pd->redirect_file != NULL) {
        pd->redirect = 1;
    }


    // Parse input into command and arguments.
    command[0] = strtok_r(input, " ", &saveptr);

    while (command[i-1] != NULL) {
        command[i] = strtok_r(NULL, " ", &saveptr);
        i = i + 1;
    }
}

void process(char *command[], struct parsed_data *pd) {

    char pathname[PATHNAME_LEN];
    strcpy(pathname, path);


    // Built-in exit command
    if (strcmp(command[0], "exit") == 0) {
        if (command[1] != NULL) {
            fprintf(stderr, "An error has occurred\n");
            return;
        } else {
            _exit(EXIT_SUCCESS);
        }
    }

    // Built-in cd command
    if (strcmp(command[0], "cd") == 0) {
        if (command[1] != NULL) {
            if (chdir(command[1]) == -1) {
                fprintf(stderr, "An error has occurred\n");
            }
        } else {
            fprintf(stderr, "An error has occurred\n");
        }
        return;
    }

    // Built-in path command
    if (strcmp(command[0], "path") == 0) {
        // If path is invoked with 
        if (command[1] == NULL) {
            path[0] = '\0';
        } else {
            strcpy(path, command[1]);
        }
        return;
    }

    // Create a new process and replace it with user
    // requested process.
    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "Could not spawn process.");
    } else if (rc == 0) {
        // Get correct value of pathname
        if (path == NULL) {
            strcpy(pathname, command[0]);
        } else {
            if (path[strlen(path) - 1] != '/') {
                strcat(pathname, "/");
            }
            strcat(pathname, command[0]);
        }

        // Before we replace the process image we 
        // will open the pipes for redirection support.
        
        int dest_fd = open(pd->redirect_file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

        if (dest_fd < 0) {
            fprintf(stderr, "An error has occurred");
            return;
        }

        int pipe1_fd[2];
        pipe1_fd[0] = 1;
        pipe1_fd[1] = dest_fd;

        int pipe2_fd[2];
        pipe2_fd[0] = 2;
        pipe2_fd[1] = dest_fd;

        if (pipe2(pipe1_fd, O_CLOEXEC) < 0) {
            fprintf(stderr, "An error has occurred");
            return;

        }

        if (pipe2(pipe2_fd, O_CLOEXEC) < 0) {
            fprintf(stderr, "An error has occurred");
            return;
        }

        int exec_code = execv(pathname, command);
        if (exec_code < 0) {
            fprintf(stderr, "An error has occurred\n");
            _exit(EXIT_FAILURE);
        }
    } else {
        wait(NULL);
    }
}