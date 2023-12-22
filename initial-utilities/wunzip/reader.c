#include <stdio.h>
#include <stdlib.h>

int main() {

  FILE *db;

  db = fopen("tests/1.in", "r");
  int nr;

  fread(&nr, sizeof(int), 1, db);

  printf("%d\n", nr);

  fclose(db);
}
