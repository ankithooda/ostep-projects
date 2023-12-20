#include <stdio.h>
#include <stdlib.h>

char *usage_message = "wcat : wcat <input>";
char *file_open_error = "wcat: cannot open file";
char *file_read_error = "wcat: cannot read from input file";
char *file_write_error = "wcat: cannot write to output file";

int main(int argc, char **argv) {
  FILE *input = NULL, *output = stdout;
  size_t buflen = BUFSIZ, read_count = 0, write_count = 0;
  char buffer[buflen];        // Allocate buffer on stack

  if (argc < 2) {
    exit(EXIT_SUCCESS);
  } else {
    for (int i = 1; i < argc; i++) {
      input = fopen(argv[i], "r");

      if (input == NULL) {
        fprintf(output, "%s\n", file_open_error);
        exit(EXIT_FAILURE);
      }

      // Till will reach EOF on the input file.
      // We read from input and write to output.

      while (feof(input) == 0) {

        read_count = fread(buffer, sizeof(char), buflen, input);

        if (ferror(input) != 0) {
          fprintf(stderr, "%s\n", file_read_error);
          exit(EXIT_FAILURE);
        }

        write_count = fwrite(buffer, sizeof(char), read_count, output);

        if (ferror(output) != 0 || write_count != read_count) {
          fprintf(stderr, "%s\n", file_write_error);
          exit(EXIT_FAILURE);
        }
      }
    }
  }
  exit(EXIT_SUCCESS);
}
