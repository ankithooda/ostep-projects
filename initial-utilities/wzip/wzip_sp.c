#include <stdio.h>
#include <stdlib.h>

char *usage_message = "wzip: file1 [file2 ...]";
char *file_open_err_message = "wzip: cannot open file";
char *file_read_err_message = "wzip: cannot read file";

struct datum {
  int n;
  char c;
};

void compress(FILE *input);

int main(int argc, char **argv) {

  FILE *input = NULL;

  if (argc == 1) {
    fprintf(stderr, "%s\n", usage_message);
    exit(EXIT_FAILURE);
  } else {

    for (int i = 1; i < argc; i++) {
      input = fopen(argv[i], "r");

      if (input == NULL) {
        fprintf(stderr, "%s\n", file_open_err_message);
        exit(EXIT_FAILURE);
      }
      compress(input);
      fclose(input);
    }
    exit(EXIT_SUCCESS);
  }
}

void compress(FILE *input) {
  char in_buffer[BUFSIZ];

  // In the worst case where the file has non-repeating
  // characters, each character will become a datum.
  struct datum out_buffer[BUFSIZ];

  int read_count = 0, pos = 0;


  while (feof(input) == 0) {

    read_count = fread(in_buffer, sizeof(char), BUFSIZ, input);

    if (ferror(input) != 0) {
      fprintf(stderr, "%s\n", file_read_err_message);
      exit(EXIT_FAILURE);
    }

    // initialize first_datum in the out_buffer
    out_buffer[0].n = 1;
    out_buffer[0].c = in_buffer[0];

    for (int i = 1; i < read_count; i++) {

      // If there is an active count for incoming character
      // we increment the count.
      // otherwise we start a new active count.
      if (out_buffer[pos].c == in_buffer[i]) {
        out_buffer[pos].n++;
      } else {

        // If out_buffer is filled, we write out to stdout
        // and then resume counting.
        if (pos >= BUFSIZ) {
          fwrite(out_buffer, sizeof(struct datum), pos + 1, stdout);
          pos = -1;
        }
        pos++;
        out_buffer[pos].c = in_buffer[i];
        out_buffer[pos].n = 1;
      }
    }
  }
  if (pos != -1) {
    fwrite(out_buffer, sizeof(struct datum), pos + 1, stdout);
  }
}
