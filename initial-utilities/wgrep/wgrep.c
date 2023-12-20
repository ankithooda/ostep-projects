#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

char *usage_message = "wgrep: searchterm [file ...]";
char *file_error_message = "wgrep: cannot open file";

void search(char *search_term, FILE *input);

int main(int argc, char **argv) {

  FILE *input = NULL;

  if (argc <= 1) {
    fprintf(stdout, "%s\n", usage_message);
    exit(EXIT_FAILURE);
  } else if (argc == 2) {
    search(argv[1], stdin);
    exit(EXIT_SUCCESS);
  } else {
    for (int i = 2; i < argc; i++) {
      input = fopen(argv[i], "r");

      if (input == NULL) {
        fprintf(stdout, "%s\n", file_error_message);
        exit(EXIT_FAILURE);
      }

      search(argv[1], input);
      fclose(input);
    }
    exit(EXIT_SUCCESS);
  }
}

void search(char *search_term, FILE *input) {
  char *line = NULL;
  size_t len = 0, text_i, search_i;
  bool match = false;

  while (getline(&line, &len, input) != -1) {

    text_i = 0;
    search_i = 0;
    match = false;
    while(line[text_i] != '\0' && search_term[search_i] != '\0') {

      if (line[text_i] == search_term[search_i]) {
        match = true;
        search_i++;
      } else {
        match = false;
        search_i = 0;
      }
      text_i++;
    }
    if (match) {
      fprintf(stdout, "%s", line);
    }
    //free(line);
  }
}
