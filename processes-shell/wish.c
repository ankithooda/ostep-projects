#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
#define COMMAND_TOKENS 101            // Maximum 100 tokens per are supported + 1 NUL Terminator

// Static variable for storing path.
char shellpath[PATH_LEN];

// Struct representing the parsed command.
struct shell_command {
    char *command[COMMAND_TOKENS];
    char *redirect_file;
    bool background;
    struct shell_command *next;
};

// Error message
char error_message[30] = "An error has occurred\n";

struct shell_command **parse_input(char *);
void process(struct shell_command **);
char *strtrim(char *);

char *strtrim(char *s) {

    if (s == NULL) {return NULL;}

    // Trim all leading whitespace
    while (s[0] == ' ' || s[0] == '\t' || s[0] == '\n') {
        s = s + sizeof(char);
    }

    int i = 0;
    int end = -1;

    // End stores the location of last whitespace char.
    while (s[i] != '\0') {
        if (s[i] == ' ' || s[i] == '\t' || s[i] == '\n') {
            end = i;
        } else {
            end = -1;
        }
        i++;
    }

    // Terminate string at the last whitespace char.
    if (end != -1) {
        s[end] = '\0';
    }
    return s;

}

int main(int argc, char *argv[]) {

    char *input;                // input buffer
    FILE *stream;               // File object to represent input file or stdin.
    size_t len = 0;

    struct shell_command **pd;
    // Intialize path to the bin directory.
    strcpy(shellpath, "/bin/");

    // Set appropriate value for the input stream.
    if (argc == 1) {
        stream = stdin;
    } else if (argc == 2) {
        stream = fopen(argv[1], "r+");
        if (stream == NULL) {
            fprintf(stderr, "%s", error_message);
            exit(EXIT_FAILURE);            
        }
    } else {
        fprintf(stderr, "%s", error_message);
        exit(EXIT_FAILURE);
    }
    while (1) {
        // Print prompt only in interactive mode.
        if (argc == 1) { printf("wish> "); }
        
        if (getline(&input, &len, stream) != -1) {
            if (strlen(input) > 1) {
                pd = parse_input(input);
                process(pd);
            }
        } else {
            free(input);
            exit(EXIT_SUCCESS);
        }
    }
    // TODO: Handle SIGINT to free memory.
    return 0;
}

struct shell_command **parse_input(char *input) {


    char *command, *command_args, *command_arg;
    int args_index;
    
    // Remove trailing newline
    input[strcspn(input, "\n")] = '\0';

    struct shell_command **pc = malloc(sizeof (struct shell_command*));
    struct shell_command *current;


    while (input != NULL) {
        if (*pc == NULL) {
            *pc = malloc(sizeof(struct shell_command));
            current = *pc;
        } else {
            current->next = malloc(sizeof(struct shell_command));
            current = current->next;
        }

        if (strchr(input, '&') != NULL) {
            current->background = true;
        }
        command = strsep(&input, "&");
        command_args = strsep(&command, ">");

        // Trim leading and trailing whitespaces.
        command = strtrim(command);

        // Get redirect file name
        current->redirect_file = strsep(&command, " ");

        // Command now contains the second redirect_file
        if (command != NULL && command[0] != '\0') {
            fprintf(stderr, "%s", error_message);

            // TODO: Fix this abomination.
            exit(EXIT_SUCCESS);
        }

        args_index = 0;

        while (command_args != NULL) {
            command_arg = strsep(&command_args, " ");

            // If we get a zero length string i.e first char is \0
            // We do not add it to parsed data structure.
            if (command_arg[0] != '\0') {
                current->command[args_index] = strtrim(command_arg);
                args_index = args_index + 1;
            } 

        }
        current->command[args_index] = NULL;
    }
    return pc;
}

void process(struct shell_command **pd) {

    struct shell_command *current;
    current = *pd;
    char process_binary[PATH_LEN];

    while (current != NULL) {
        // Return if no command is passed.
        if (current->command[0] == NULL) {
            // Print error message if there was a redirect file.
            if (current->redirect_file != NULL) {
                fprintf(stderr, "%s", error_message);
            }
            return;
        }


        // Built-in exit command
        if (strcmp(current->command[0], "exit") == 0) {
            if (current->command[1] != NULL) {
                fprintf(stderr, "%s", error_message);
                return;
            } else {
                _exit(EXIT_SUCCESS);
            }
        }

        // Built-in cd command
        if (strcmp(current->command[0], "cd") == 0) {
            if (current->command[1] != NULL) {
                if (chdir(current->command[1]) == -1) {
                    fprintf(stderr, "%s", error_message);
                }
            } else {
                fprintf(stderr, "%s", error_message);
            }
            return;
        }

        // Built-in path command
        if (strcmp(current->command[0], "path") == 0) {
            // If path is invoked with 
            if (current->command[1] == NULL) {
                shellpath[0] = '\0';
            } else {
                strcpy(shellpath, current->command[1]);
                if (shellpath[strlen(shellpath) - 1] != '/') {
                    strcat(shellpath, "/");
                }
            }
            return;
        }

        // Create a new process and replace it with user
        // requested process.
        int rc = fork();

        if (rc < 0) {
            fprintf(stderr, "%s", error_message);
        } else if (rc == 0) {
            // Get correct value of pathname
            if (shellpath == NULL) {
                strcpy(process_binary, current->command[0]);
            } else {
                strcpy(process_binary, shellpath);
                strcat(process_binary, current->command[0]);
            }

            // Before we replace the process image we 
            // will open the pipes for redirection support.
            
            if (current->redirect_file != NULL) {

                // If redirect file name is empty
                // print error and exit the child process.
                if (current->redirect_file[0] == '\0') {
                    fprintf(stderr, "%s", error_message);
                    _exit(EXIT_FAILURE);
                }

                // Close STDOUT and STDERR
                close(STDOUT_FILENO);
                close(STDERR_FILENO);

                // TODO: find a better a way to handle
                // errors when opening redirect file.
                int stdout_fd = open(current->redirect_file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
                int stderr_fd = open(current->redirect_file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

                // This message get never printed as STDOUT and STDERR are closed
                // for the child process.
                if (stdout_fd < 0 || stderr_fd < 0) {
                    fprintf(stderr, "%s", error_message);
                }
            }

            int exec_code = execv(process_binary, current->command);
            if (exec_code < 0) {
                fprintf(stderr, "%s", error_message);
                _exit(EXIT_FAILURE);
            }
        } else {
            if (current->background != true) {
                waitpid(rc, NULL, 0);
            }
        }
        current = current->next;
    }
}