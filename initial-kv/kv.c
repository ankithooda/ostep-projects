#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define PUT 'p'
#define GET 'g'
#define DEL 'd'
#define CLR 'c'
#define ALL 'a'

char *db_file = "database.bin";
char *bad_command = "bad command";
char *file_open_error = "kv: cannot open db file";
char *file_read_error = "kv: cannot read db file";
char *file_write_error = "kv: cannot write db file";

struct command {
  char type;
  int key;
  char *value;
};

struct pair {
  int key;
  char *value;
};

void process_get(struct command input);
void process_put(struct command input);
void process_del(struct command input);
void process_all(void);
void process_clr(void);
struct command *parse_command(char *input);

void kv_write(int k, char *v, FILE *db);
struct pair kv_read(FILE *db);


int main(int argc, char **argv) {
  struct command *c;
  if (argc == 1) {
    exit(EXIT_SUCCESS);
  } else {
    for (int i = 1; i < argc; i++) {
      c = parse_command(argv[i]);

      switch(c->type) {
      case GET:
        process_get(*c);
        break;
      case PUT:
        process_put(*c);
        break;
      case DEL:
        process_del(*c);
        break;
      case CLR:
        process_clr();
        break;
      case ALL:
        process_all();
        break;
      default:
        fprintf(stdout, "%s\n", "kv: crash and burn");
        free(c);
        exit(EXIT_FAILURE);
      }
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
  if (c->type == PUT && c->value == NULL) {
    fprintf(stdout, "%s\n", bad_command);
    exit(EXIT_FAILURE);
  }

  return c;
}

void kv_write(int k, char *v, FILE *db) {
  int len = strlen(v);

  fwrite(&k, sizeof(int), 1, db);
  fwrite(&len, sizeof(int), 1, db);
  fwrite(&v, sizeof(char), len, db);

  fflush(db);
}

struct pair kv_read(FILE *db) {
  struct pair p;
  int len;
  fread(&p.key, sizeof(int), 1, db);
  fread(&len, sizeof(int), 1, db);
  p.value = malloc(len+1);       // For \0 at the end.
  p.value[len] = '\0';
  fread(p.value, sizeof(char), len, db);
  return p;
}


void process_put(struct command input) {
  FILE *db = fopen(db_file, "a");

  if (db == NULL) {
    fprintf(stderr, "%s\n", file_open_error);
    exit(EXIT_FAILURE);
  }
  kv_write(input.key, input.value, db);

  fclose(db);
}

void process_get(struct command input) {;}
void process_del(struct command input) {;}

void process_clr(void) {

  FILE *db = fopen(db_file, "w");

  if (db == NULL) {
    fprintf(stderr, "%s\n", file_open_error);
    exit(EXIT_FAILURE);
  }

  if (ftruncate(fileno(db), 0) == -1) {
    fprintf(stderr, "%s\n", file_write_error);
    fclose(db);
    exit(EXIT_FAILURE);
  } else {
    fclose(db);
    exit(EXIT_SUCCESS);
  }
}

void process_all(void) {
  FILE *db = fopen(db_file, "r");

  if (db == NULL) {
    fprintf(stderr, "%s\n", file_open_error);
    exit(EXIT_FAILURE);
  }

  fclose(db);
  exit(EXIT_SUCCESS);
}
