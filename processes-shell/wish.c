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

struct parsed_data **parse_input(char *);
void process(char *[], struct parsed_data *);

int main(int argc, char *argv[]) {

    char *input;                // input buffer
    char *command[INPUT_LEN];   // Tokenized input
    FILE *stream;               // File object to represent input file or stdin.
    size_t len = 0;

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
                parse_input(input, pd);
                // process(command, &pd);
            }
        } else {
            free(input);
            exit(EXIT_SUCCESS);
        }
    }
    // TODO: Handle SIGINT to free memory.
    return 0;
}

struct parsed_data ** parse_input(char *input) {


    char *command, *command_args, *command_arg;
    int command_index = 0;
    int args_index = 0;
    
    // Remove trailing newline
    input[strcspn(input, "\n")] = '\0';

    // Parse input for multiple commands with background directive.

    struct parsed_command *pc;
    struct parsed_command *current;


    while (input != NULL) {
        pc = malloc(sizeof(struct parsed_command));
        current = pc;

        command = strsep(&input, "&");
        command_args = strsep(&command, ">");

        pd[command_index].redirect_file = command;

        while (command_args != NULL) {
            command_arg = strsep(&command_args, " ");
            pd[command_index].command[args_index] = command_arg;

            args_index = args_index + 1;
        }
        command_index = command_index + 1;
            
    }
        
        // printf("TOKEN %s\n", token);
        // printf("INPUT %s\n", input);
        // printf("------------------------------------->\n");
        
        // printf("SUBTOKEN %s\n", subtoken);
        // printf("TOKEN %s\n", token);
        // printf("#####################################\n");
    
    
    
    // while (token != NULL) {
    //     // Each iteration parses a single command.

    //     // Parse input for redirection.
    //     token = strtok_r(token, ">", &subtoken_saveptr);
    //     *pd->redirect_file = strtok_r(NULL, ">", &subtoken_saveptr);
    //     if (pd->redirect_file != NULL) {
    //         pd->redirect = 1;
    //     }

    //     // Parse input into command and arguments.
    //     pd[j]->command[i] = strtok_r(token, " ", &subtoken_saveptr);

    //     while (pd[0]->command[i-1] != NULL) {
    //         pd[0]->command[i] = strtok_r(NULL, " ", &saveptr);
    //         i = i + 1;
    //     }
    // }    
}

// void process(char *command[], struct parsed_data **pd) {

//     char pathname[PATH_LEN];
//     strcpy(pathname, path);


//     // Built-in exit command
//     if (strcmp(command[0], "exit") == 0) {
//         if (command[1] != NULL) {
//             fprintf(stderr, "An error has occurred\n");
//             return;
//         } else {
//             _exit(EXIT_SUCCESS);
//         }
//     }

//     // Built-in cd command
//     if (strcmp(command[0], "cd") == 0) {
//         if (command[1] != NULL) {
//             if (chdir(command[1]) == -1) {
//                 fprintf(stderr, "An error has occurred\n");
//             }
//         } else {
//             fprintf(stderr, "An error has occurred\n");
//         }
//         return;
//     }

//     // Built-in path command
//     if (strcmp(command[0], "path") == 0) {
//         // If path is invoked with 
//         if (command[1] == NULL) {
//             path[0] = '\0';
//         } else {
//             strcpy(path, command[1]);
//         }
//         return;
//     }

//     // Create a new process and replace it with user
//     // requested process.
//     int rc = fork();

//     if (rc < 0) {
//         fprintf(stderr, "Could not spawn process.");
//     } else if (rc == 0) {
//         // Get correct value of pathname
//         if (path == NULL) {
//             strcpy(pathname, command[0]);
//         } else {
//             if (path[strlen(path) - 1] != '/') {
//                 strcat(pathname, "/");
//             }
//             strcat(pathname, command[0]);
//         }

//         // Before we replace the process image we 
//         // will open the pipes for redirection support.
        
//         if (pd->redirect == 1) {
//             close(STDOUT_FILENO);
//             close(STDERR_FILENO);

//             int stdout_fd = open(pd->redirect_file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
//             int stderr_fd = open(pd->redirect_file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

//             if (stdout_fd < 0 || stderr_fd < 0) {
//                 fprintf(stderr, "An error has occurred");
//             }
//         }

//         int exec_code = execv(pathname, command);
//         if (exec_code < 0) {
//             fprintf(stderr, "An error has occurred\n");
//             _exit(EXIT_FAILURE);
//         }
//     } else {
//         wait(NULL);
//     }
// }