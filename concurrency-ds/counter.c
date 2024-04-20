#include <stdio.h>
#include <assert.h>
#include <pthread.h>

#define MILLION 1000000
#define THREAD_COUNT 256


typedef struct __counter_ {
  unsigned long count;
  pthread_mutex_t lock;
} counter_t;

counter_t c;

void init(counter_t *c) {
  c->count = 0;
  assert(pthread_mutex_init(&c->lock, NULL) == 0);
}

void destroy(counter_t *c) {
  assert(pthread_mutex_destroy(&c->lock) == 0);
}

void increment(counter_t *c) {
  assert(pthread_mutex_lock(&c->lock) == 0);
  c->count++;
  assert(pthread_mutex_unlock(&c->lock) == 0);
}

void decrement(counter_t *c) {
  assert(pthread_mutex_lock(&c->lock) == 0);
  c->count++;
  assert(pthread_mutex_unlock(&c->lock) == 0);
}

unsigned long get_count(counter_t *c) {
  int rc;
  assert(pthread_mutex_lock(&c->lock) == 0);
  rc = c->count;
  assert(pthread_mutex_unlock(&c->lock) == 0);
  return rc;
}

void *mythread(void *arg) {
  for (int i = 0; i < MILLION; i++)
    increment(&c);
  return NULL;
}


int main() {
  pthread_t threads[THREAD_COUNT];
  init(&c);

  for (int i = 0; i < THREAD_COUNT; i++)
    assert(pthread_create(&threads[i], NULL, mythread, NULL) == 0);

  for (int i = 0; i < THREAD_COUNT; i++)
    assert(pthread_join(threads[i], NULL) == 0);

  printf("Final Count %lu\n", get_count(&c));
  destroy(&c);
  return 0;
}
