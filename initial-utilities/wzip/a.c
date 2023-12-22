#include <stdio.h>

struct datum {
  int n;
  char c;
};

int main() {
  struct datum d[10];
  d[0].n = 123;
  d[0].c = 'X';
  d[1].n = 123;
  d[1].c = 'X';

  printf("%zu,%zu,%zu,%zu,%zu\n", sizeof(d[0].c), sizeof(d[0].n), sizeof(char), sizeof('X'),sizeof(struct datum));

  //fwrite(d, 5, 2, stdout);
  fprintf(stdout, "0b%d%c", d[0].n, d[0].c);
  return 0;
}
