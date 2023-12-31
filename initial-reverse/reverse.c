#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define INITIAL_BUFFER_LEN 2

char *usage_message = "usage: reverse <input> <output>";
char *file_error_template = "reverse: cannot open file '%s'\n";
char *file_same_error_message = "reverse: input and output file must differ";
char *malloc_failed_message = "malloc failed";

int main(int argc, char **argv) {

  FILE *input, *output;
  char **lines = NULL;
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

    struct stat input_stat, output_stat;

    // If we are not able to stat input file,
    // we will print error message and exit.
    if (stat(argv[1], &input_stat) == -1) {
      fprintf(stderr, file_error_template, argv[1]);
      exit(EXIT_FAILURE);
    }

    // We do not care about stat on output file
    // as it might no exist.
    stat(argv[2], &output_stat);

    // Check if inode number of both files is same.
    // Inode number is a robust way of checking two files
    // are same (soft links, hard links, same name)
    if (input_stat.st_ino == output_stat.st_ino) {
      fprintf(stderr, "%s\n", file_same_error_message);
      exit(EXIT_FAILURE);
    }

    // Open files
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
  if (lines == NULL) {
    fprintf(stderr, "%s\n", malloc_failed_message);
    free(lines);                 // When reallocarray fails it does no free old memory.
    exit(EXIT_FAILURE);
  }
  // Getline from the input
  // store the pointers to each line in a buffer.
  while ((line_num < buffer_len) && (getline(&lines[line_num], &line_len, input) != -1)) {
    line_num++;
    // If we have more lines than the current buffer_len
    // we double the buffer size;
    if (line_num >= buffer_len) {
      buffer_len = buffer_len * 2;
      lines = reallocarray(lines, buffer_len, sizeof(char *));
      if (lines == NULL) {
        fprintf(stderr, "%s\n", malloc_failed_message);
        free(lines);              // When reallocarray fails it does no free old memory.
        exit(EXIT_FAILURE);
      }
    }
  }

  // Exit if there were no lines.
  if (line_num == 0) {
    exit(EXIT_SUCCESS);
  }

  // Write the lines stored in the buffer
  // to the output file in reverse order.
  do {
    line_num--;
    fprintf(output, "%s", lines[line_num]);

    // Call free after each line is printed out.
    free(lines[line_num]);
  }
  while (line_num != 0);

  // Finally free the memory and close open files.
  free(lines);
  fclose(input);
  fclose(output);

  exit(EXIT_SUCCESS);
}
