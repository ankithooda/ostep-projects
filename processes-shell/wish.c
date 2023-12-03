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
#define INPUT_LEN 10000               // Maximum input by user can be 10000 chars
#define PATH_LEN 100                  // Path len can be 100 chars

// TODO: Dynamically allocate this.
#define COMMANDS 101                  // Maximum 100 Commands + 1 NUL Terminator
#define COMMAND_TOKENS 101            // Maximum 100 tokens per are supported + 1 NUL Terminator

// Static variable for storing path.
char path[PATH_LEN];

// Struct representing the parsed command.
struct parsed_command {
    char *command[COMMAND_TOKENS];
    char *redirect_file;
    struct parsed_command *next;
};

// Error message
char error_message[30] = "An error has occurred\n";

struct parsed_command **parse_input(char *);
void process(char *[], struct parsed_command **);

int main(int argc, char *argv[]) {

    char *input;                // input buffer
    char *command[INPUT_LEN];   // Tokenized input
    FILE *stream;               // File object to represent input file or stdin.
    size_t len = 0;

    struct parsed_command **pd;
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
        if (argc == 1) { printf("wish> "); }
        
        if (getline(&input, &len, stream) != -1) {
            if (strlen(input) > 1) {
                pd = parse_input(input);
                ;
                process(command, pd);
            }
        } else {
            free(input);
            exit(EXIT_SUCCESS);
        }
    }
    // TODO: Handle SIGINT to free memory.
    return 0;
}

struct parsed_command **parse_input(char *input) {


    char *command, *command_args, *command_arg;
    int args_index = 0;
    
    // Remove trailing newline
    input[strcspn(input, "\n")] = '\0';

    // Parse input for multiple commands with background directive.

    struct parsed_command **pc = malloc(sizeof (struct parsed_command*));
    struct parsed_command *current;


    while (input != NULL) {
        if (*pc == NULL) {
            *pc = malloc(sizeof(struct parsed_command));
            current = *pc;
        } else {
            current->next = malloc(sizeof(struct parsed_command));
            current = current->next;
        }

        command = strsep(&input, "&");
        command_args = strsep(&command, ">");

        current->redirect_file = command;

        while (command_args != NULL) {
            command_arg = strsep(&command_args, " ");
            current->command[args_index] = command_arg;

            args_index = args_index + 1;
        }
    }
    return pc;
}

void process(char *command[], struct parsed_command **pd) {

    struct parsed_command *current;
    current = *pd;
    while (current != NULL) {
        int i = 0;
        printf("----- \n%s\n", current->redirect_file);
        while(current->command[i] != NULL) {
            printf("%s\n", current->command[i]);
            i++;
        }
        printf("*********");
    }
    ;

    // char pathname[PATH_LEN];
    // strcpy(pathname, path);


    // // Built-in exit command
    // if (strcmp(pd->command[0], "exit") == 0) {
    //     if (pd->command[1] != NULL) {
    //         fprintf(stderr, error_message);
    //         return;
    //     } else {
    //         _exit(EXIT_SUCCESS);
    //     }
    // }

    // // Built-in cd command
    // if (strcmp(pd->command[0], "cd") == 0) {
    //     if (pd->command[1] != NULL) {
    //         if (chdir(pd->command[1]) == -1) {
    //             fprintf(stderr, error_message);
    //         }
    //     } else {
    //         fprintf(stderr, error_message);
    //     }
    //     return;
    // }

    // // Built-in path command
    // if (strcmp(pd->command[0], "path") == 0) {
    //     // If path is invoked with 
    //     if (pd->command[1] == NULL) {
    //         path[0] = '\0';
    //     } else {
    //         strcpy(path, pd->command[1]);
    //     }
    //     return;
    // }

    // // Create a new process and replace it with user
    // // requested process.
    // int rc = fork();

    // if (rc < 0) {
    //     fprintf(stderr, error_message);
    // } else if (rc == 0) {
    //     // Get correct value of pathname
    //     if (path == NULL) {
    //         strcpy(pathname, pd->command[0]);
    //     } else {
    //         if (path[strlen(path) - 1] != '/') {
    //             strcat(pathname, "/");
    //         }
    //         strcat(pathname, pd->command[0]);
    //     }

    //     // Before we replace the process image we 
    //     // will open the pipes for redirection support.
        
    //     if (pd->redirect == 1) {
    //         close(STDOUT_FILENO);
    //         close(STDERR_FILENO);

    //         int stdout_fd = open(pd->redirect_file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    //         int stderr_fd = open(pd->redirect_file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    //         if (stdout_fd < 0 || stderr_fd < 0) {
    //             fprintf(stderr, error_message);
    //         }
    //     }

    //     int exec_code = execv(pathname, command);
    //     if (exec_code < 0) {
    //         fprintf(stderr, error_message);
    //         _exit(EXIT_FAILURE);
    //     }
    // } else {
    //     wait(NULL);
    // }
}