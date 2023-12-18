#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define INITIAL_BUFFER_LEN 2

char *usage_message = "usage: reverse <input> <output>";
char *file_error_template = "error: cannot open file '%s'\n";
char *file_same_error_message = "Input and output file must differ";
char *malloc_failed_message = "malloc failed";

int main(int argc, char **argv) {

  FILE *input, *output;
  char **lines = NULL;            // Allocate a buffer for storing pointers to the lines.
  char **temp_buffer;
  size_t line_len;
  size_t line_num = 0;
  size_t buffer_len = INITIAL_BUFFER_LEN;


  // No Input or Output files are given.
  if (argc == 1) {
    input = stdin;
    output = stdout;

  } else if (argc == 2) {
    input = fopen(argv[1], "r");
    output = stdout;

  } else if (argc == 3) {
    // Before we open the input and output file
    // we will check if they are both the same file.
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

  // Allocate initial buffer
  lines = reallocarray(lines, buffer_len, sizeof(char *));

  // Getline from the input
  // store the pointers to each line in a buffer.
  while ((line_num < buffer_len) && (getline(&lines[line_num], &line_len, input) != -1)) {
    line_num++;
    // If we have more lines than the current buffer_len
    // we double the buffer size;
    if (line_num >= buffer_len) {
      temp_buffer = lines;
      lines = reallocarray(lines, buffer_len * 2, sizeof(char *));
      if (lines == NULL) {
        fprintf(stderr, "%s\n", malloc_failed_message);
        exit(EXIT_FAILURE);
      }
      memcpy(lines, temp_buffer, buffer_len * sizeof(char *));

      buffer_len = buffer_len * 2;
      free(temp_buffer);
    }
  }

  // Write the lines stored in the buffer
  // to the output file in reverse order.
  while (line_num >= 0) {
    fprintf(output, "%s", lines[line_num]);
    line_num--;
  }


  fclose(input);
  fclose(output);
  exit(EXIT_SUCCESS);
}
