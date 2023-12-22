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
  char *type_token, *key_token;

  c = malloc(sizeof(struct command));

  type_token = strsep(&input, ",");
  key_token = strsep(&input, ",");
  c->value = strsep(&input, ",");

  // Validate type
  if (type_token != NULL && (type_token[0] == GET || type_token[0] == PUT || type_token[0] == DEL || type_token[0] == CLR || type_token[0] == ALL)) {
    c->type = type_token[0];
  } else {
    fprintf(stdout, "%s\n", bad_command);
    exit(EXIT_FAILURE);
  }

  // Validate key for PUT, GET, DEL
  if (c->type == GET || c->type == PUT || c->type == DEL) {
    if (key_token == NULL) {
      fprintf(stdout, "%s\n", bad_command);
      exit(EXIT_FAILURE);
    } else {
      c->key = atoi(key_token);
    }
  }

  // Validate value for PUT
  if ((c->type == GET ||  c->type == PUT || c->type == DEL) && c->value == NULL) {
    fprintf(stdout, "%s\n", bad_command);
    exit(EXIT_FAILURE);
  }

  return c;
}

void process(struct command input) {
  ;
}
