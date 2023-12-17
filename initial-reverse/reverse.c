#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char *usage_message = "usage: reverse <input> <output>";
int main(int argc, char **argv) {
  printf("Hello World!\n");
  printf("No. of arguments %d\n", argc);

  for (int i = 0; i < argc; i++) {
    printf("%s\n", argv[i]);
  }

  FILE *input, *output;
  char *line;
  size_t line_len;
  // No Input or Output files are given.
  if (argc == 1) {
    input = stdin;
    output = stdout;
  } else if (argc == 2) {
    input = fopen(argv[1], "r");
  } else if (argc == 3) {
    input = fopen(argv[1], "r");
    output = fopen(argv[2], "w");
  } else {
    fprintf(stderr, "%s\n", usage_message);
  }

  if (input == NULL) {
    fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  if (output == NULL) {
    fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
    exit(EXIT_FAILURE);
  }

  while (getline(&line, &line_len, input) != -1) {
    printf("line_len %zu\n", line_len);
    fprintf(output, "%s", line);
  }

  fclose(input);
  fclose(output);
  exit(EXIT_SUCCESS);
}
