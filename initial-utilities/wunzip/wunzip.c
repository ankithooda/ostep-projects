#include <stdio.h>
#include <stdlib.h>

void uncompress(FILE *input);

int main(int argc, char **argv) {
  FILE *input;

  if (argc == 1) {
    fprintf(stdout, "%s\n", "wunzip: file1 [file2 ...]");
    exit(EXIT_FAILURE);
  } else {

    for (int i = 1; i < argc; i++) {

      input = fopen(argv[i], "r");

      if (input == NULL) {
        fprintf(stdout, "%s\n", "wunzip: cannot open file");
        exit(EXIT_FAILURE);
      }
      uncompress(input);
      fclose(input);
    }
    exit(EXIT_SUCCESS);
  }
}

void uncompress(FILE *input) {
  int n;
  char c;

  while (feof(input) == 0) {

    fread(&n, sizeof(int), 1, input);
    fread(&c, sizeof(char), 1, input);

    if (ferror(input) != 0) {
      fprintf(stderr, "%s\n", "wunzip: cannot read file");
      exit(EXIT_FAILURE);
    }
    /*
    if (feof(input) != 0) {
      fprintf(stderr, "%s\n", "wunzip: invalid zip format");
      exit(EXIT_FAILURE);
    }
    */
    for (int i = 0; i < n; i++) {
      printf("%c", c);
    }
    ;
  }
}
