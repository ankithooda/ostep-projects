#include <stdio.h>
#include <assert.h>
#include <pthread.h>

#define MILLION 1000000
#define THREAD_COUNT 8
#define NUM_CPUS 8
#define THRESHOLD 100000

typedef struct __counter_ {
  // Global count and Global lock
  unsigned long gcount;
  pthread_mutex_t glock;

  // CPU counts and CPU locks
  unsigned long lcount[NUM_CPUS];
  pthread_mutex_t llock[NUM_CPUS];

  // Threshold after which the local cpu counts are
  // added in the global count.
  unsigned int threshold;
} counter_t;

counter_t c;

void init(counter_t *c) {
  c->gcount = 0;
  assert(pthread_mutex_init(&c->glock, NULL) == 0);

  for (int i = 0; i < NUM_CPUS; i++) {
    c->lcount[i] = 0;
    assert(pthread_mutex_init(&c->llock[i], NULL) == 0);
  }
}

void destroy(counter_t *c) {
  assert(pthread_mutex_destroy(&c->glock) == 0);
  for (int i = 0; i < NUM_CPUS; i++) {
    assert(pthread_mutex_destroy(&c->llock[i]) == 0);
  }
}

void update(counter_t *c, unsigned int thread_id, long a) {
  // First increment the local count
  int i = thread_id / NUM_CPUS;
  assert(pthread_mutex_lock(&c->llock[i]) == 0);
  c->lcount[i] += a;

  // Check if local count is greater than threshold
  if (c->lcount[i] >= c->threshold) {
    assert(pthread_mutex_lock(&c->glock) == 0);
    c->gcount += c->lcount[i];
    c->lcount[i] = 0;
    assert(pthread_mutex_unlock(&c->glock) == 0);
  }
  assert(pthread_mutex_unlock(&c->llock[i]) == 0);
}

unsigned long get_count(counter_t *c) {
  int rc;
  assert(pthread_mutex_lock(&c->glock) == 0);
  rc = c->gcount;
  assert(pthread_mutex_unlock(&c->glock) == 0);
  return rc;
}

void *mythread(void *arg) {
  unsigned int id = *(unsigned int *)arg;
  printf("Started Thread: %u\n", id);
  for (int i = 0; i < MILLION; i++) {
    update(&c, id, 1);
  }
  return NULL;
}


int main() {
  pthread_t threads[THREAD_COUNT];
  init(&c);

  for (unsigned int i = 0; i < THREAD_COUNT; i++) {
    printf("Going to spawn thread %u\n", i);
    assert(pthread_create(&threads[i], NULL, mythread, (void *)&i) == 0);
  }
  for (int i = 0; i < THREAD_COUNT; i++)
    assert(pthread_join(threads[i], NULL) == 0);

  printf("Final Count %lu\n", get_count(&c));
  destroy(&c);
  return 0;
}
