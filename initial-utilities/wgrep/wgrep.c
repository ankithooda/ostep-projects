#include <stdio.h>
#include <stdlib.h>

char *usage_message = "wgrep: searchterm [file ...]";
char *file_error_message = "wgrep: cannot open file";

void search(char *search_term, FILE *input);

int main(int argc, char **argv) {

  FILE *input = NULL;

  if (argc <= 1) {
    fprintf(stderr, "%s\n", usage_message);
    exit(EXIT_FAILURE);
  } else if (argc == 2) {
    search(argv[1], stdin);
    exit(EXIT_SUCCESS);
  } else {
    for (int i = 2; i < argc; i++) {
      input = fopen(argv[i], "r");

      if (input == NULL) {
        fprintf(stderr, "%s\n", file_error_message);
        exit(EXIT_FAILURE);
      }

      search(argv[1], input);
      fclose(input);
    }
    exit(EXIT_SUCCESS);
  }
}

void search(char *search_term, FILE *input) {
  ;
}
