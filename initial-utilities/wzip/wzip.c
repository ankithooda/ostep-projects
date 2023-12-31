#include <stdio.h>
#include <stdlib.h>

char *usage_message = "wzip: file1 [file2 ...]";
char *file_open_err_message = "wzip: cannot open file";
char *file_read_err_message = "wzip: cannot read file";

struct datum {
  int n;
  char c;
};

void compress(FILE *input, struct datum *counter);

int main(int argc, char **argv) {

  FILE *input = NULL;
  struct datum *counter;

  if (argc == 1) {
    fprintf(stdout, "%s\n", usage_message);
    exit(EXIT_FAILURE);
  } else {

    // Allocate memory for counter.
    counter = malloc(sizeof (struct datum));

    if (counter == NULL) {
      fprintf(stderr, "%s\n", "wzip: malloc failed");
      exit(EXIT_FAILURE);
    }

    counter->n = 0;
    counter->c = '\0';

    for (int i = 1; i < argc; i++) {
      input = fopen(argv[i], "r");

      if (input == NULL) {
        fprintf(stderr, "%s\n", file_open_err_message);
        exit(EXIT_FAILURE);
      }
      compress(input, counter);
      fclose(input);
    }
    if (counter->n != 0 && counter->c != '\0') {
      fwrite(&counter->n, sizeof(int), 1, stdout);
      fprintf(stdout, "%c", counter->c);
    }
    free(counter);
    exit(EXIT_SUCCESS);
  }
}

void compress(FILE *input, struct datum *counter) {
  char in_buffer[BUFSIZ];

  int read_count = 0;

  while (feof(input) == 0) {

    read_count = fread(in_buffer, sizeof(char), BUFSIZ, input);

    if (ferror(input) != 0) {
      fprintf(stderr, "%s\n", file_read_err_message);
      exit(EXIT_FAILURE);
    }

    for (int i = 0; i < read_count; i++) {

      // If there is an active count for incoming character
      // we increment the count.
      // otherwise we start a new active count.
      if (counter->c == in_buffer[i]) {
        counter->n++;
      } else {
        if (counter->n != 0 && counter->c != '\0') {
          fwrite(&counter->n, sizeof(int), 1, stdout);
          fprintf(stdout, "%c", counter->c);
        }
        // Reset counter
        counter->c = in_buffer[i];
        counter->n = 1;
      }
    }
  }
}
