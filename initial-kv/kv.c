#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PUT 'p'
#define GET 'g'
#define DEL 'd'
#define CLR 'c'
#define ALL 'a'

char *db_file = "database.txt";
char *bad_command = "bad command";

struct command {
  char type;
  int key;
  char *value;
};

void process(struct command input);
struct command *parse_command(char *input);

int main(int argc, char **argv) {
  struct command *c;
  if (argc == 1) {
    exit(EXIT_SUCCESS);
  } else {
    for (int i = 1; i < argc; i++) {
      c = parse_command(argv[i]);
      process(*c);
       free(c);
    }
  }
  exit(EXIT_SUCCESS);
}

struct command *parse_command(char* input) {
  struct command *c;
  char *type;

  c = malloc(sizeof(struct command));

  type = strsep(&input, ",");
  c->key = atoi(strsep(&input, ","));
  c->value = strsep(&input, ",");

  // Validate type
  if (type != NULL && (type[0] == GET || type[0] == PUT || type[0] == DEL || type[0] == CLR || type[0] == ALL)) {
    c->type = type[0];
  } else {
    fprintf(stdout, "%s\n", bad_command);
    exit(EXIT_FAILURE);
  }

  // Validate key for PUT, GET, DEL
  if ((c->type == GET || c->type == PUT || c->type == DEL) && c->key == NULL) {
    fprintf(stdout, "%s\n", bad_command);
    exit(EXIT_FAILURE);
  }

  // Validate value for PUT
  if (c->type == PUT && c->value == NULL) {
    fprintf(stdout, "%s\n", bad_command);
    exit(EXIT_FAILURE);
  }

  return c;
}

void process(struct command input) {
  ;
}
