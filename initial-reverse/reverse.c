#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX_LINES 1000

char *usage_message = "usage: reverse <input> <output>";
char *file_error_template = "error: cannot open file '%s'\n";
char *file_same_error_message = "Input and output file must differ";

int main(int argc, char **argv) {

  FILE *input, *output;
  char *line[MAX_LINES];                     // Allocate a buffer for storing pointers to the lines.
  size_t line_len;
  long line_num = 0;

  // No Input or Output files are given.
  if (argc == 1) {
    input = stdin;
    output = stdout;

  } else if (argc == 2) {
    input = fopen(argv[1], "r");
    output = stdout;

  } else if (argc == 3) {
    // Before we open the input and output file
    // we will checkif they are both the same file.
    if (strcmp(argv[1], argv[2]) == 0) {
      fprintf(stderr, "%s\n", file_same_error_message);
      exit(EXIT_FAILURE);
    }
    input = fopen(argv[1], "r");
    output = fopen(argv[2], "w");

  } else {
    fprintf(stderr, "%s\n", usage_message);
    exit(EXIT_FAILURE);
  }

  if (input == NULL) {
    fprintf(stderr, file_error_template, argv[1]);
    exit(EXIT_FAILURE);
  }

  if (output == NULL) {
    fprintf(stderr, file_error_template, argv[2]);
    exit(EXIT_FAILURE);
  }

  // Getline from the input
  // store the pointers to each line in a buffer.
  while (getline(&line[line_num], &line_len, input) != -1 && line_num < MAX_LINES) {
    line_num++;
  }

  // Write the lines stored in the buffer
  // to the output file in reverse order.
  while(line_num >= 0) {
    fprintf(output, "%s", line[line_num]);
    line_num--;
  }

  fclose(input);
  fclose(output);
  exit(EXIT_SUCCESS);
}
