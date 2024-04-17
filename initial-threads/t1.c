#include <stdio.h>
#include <assert.h>
#include <pthread.h>

volatile int counter;

void *mythread(void *arg) {

  for (int i = 0; i < 1000000; i++) {
    counter++;
  }
  printf("Done\n");
  return NULL;
}

int
main (int argc, char *argv[]) {
  pthread_t p1, p2;
  int rc;
  printf("main: begin\n");
  counter = 0;
  printf("Counter Value %d\n", counter);

  rc = pthread_create(&p1, NULL, mythread, "A"); assert(rc == 0);
  rc = pthread_create(&p2, NULL, mythread, "B"); assert(rc == 0);

  // Join waits for thread to exit
  rc = pthread_join(p1, NULL); assert(rc == 0);
  rc = pthread_join(p2, NULL); assert(rc == 0);

  printf("Counter Value %d\n\n", counter);
  printf("main: end\n");
  return 0;
}
