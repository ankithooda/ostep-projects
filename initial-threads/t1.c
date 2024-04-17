#include <stdio.h>
#include <assert.h>
#include <pthread.h>

volatile int counter;

void *mythread(void *arg) {

  printf("thread %s: begin\n", (char *)arg);
  pthread_mutex_t lock;
  int rc = pthread_mutex_init(&lock, NULL);
  assert(rc == 0);
  assert(pthread_mutex_lock(&lock) == 0);
  printf("thread %s: lock acquired\n", (char *)arg);
  for (int i = 0; i < 1000000; i++) {
    counter++;
  }
  assert(pthread_mutex_unlock(&lock) == 0);
  printf("thread %s: Done\n", (char *)arg);
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
