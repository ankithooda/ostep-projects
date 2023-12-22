#include <stdio.h>
#include <stdlib.h>

int main() {

  FILE *db;

  db = fopen("db.bin", "w");
  int nw = 13;

  fwrite(&nw, sizeof(int), 1, db);
  fflush(db);
  fclose(db);
  printf("%d \n", nw);
}
