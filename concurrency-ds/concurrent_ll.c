#include<stdio.h>
#include<pthread.h>
#include<assert.h>
#include<stdlib.h>
#include<time.h>

#define MAX_THREAD 512
#define MILLION 1000000


typedef struct __node_t {
  int key;
  struct __node_t *next;
} node_t;

typedef struct __list_t {
  node_t *head;
  pthread_mutex_t lock;
} list_t;

list_t l;

void list_init(list_t *l) {
  l->head = NULL;
  assert(pthread_mutex_init(&l->lock, NULL) == 0);
}

void list_destroy(list_t *l) {
  node_t *t = l->head;
  node_t *n;
  while (t != NULL) {
    n = t;
    t = t->next;
    free(n);
  }
  assert(pthread_mutex_destroy(&l->lock) == 0);
}

int list_insert(list_t *l, int key) {
  node_t *new = malloc(sizeof(node_t));

  if (new == NULL) {
    return -1;
  } else {
    new->key = key;
    // Critical Section
    assert(pthread_mutex_lock(&l->lock) == 0);
    new->next = l->head;
    l->head = new;
    assert(pthread_mutex_unlock(&l->lock) == 0);
    return 0;
  }
}

unsigned int length (list_t *l) {
  unsigned int c = 0;
  node_t *n = l->head;
  while (n != NULL) {
    c++;
    n = n->next;
  }
  return c;
}

void *fill_list(void *arg) {
  //unsigned int id = *(unsigned int*)arg;
  //printf("Started Thread: %u\n", id);
  for (unsigned int i = 0; i < MILLION; i++) {
    list_insert(&l, i);
  }
  return NULL;
}

int main() {

  pthread_t thread[MAX_THREAD];
  list_init(&l);
  for (unsigned int i = 0; i < MAX_THREAD; i++) {
    assert(pthread_create(&thread[i], NULL, fill_list, NULL) == 0);
  }
  // Technically this is not correct as the main process waits
  // for first thread to complete calling pthread_join for other threads.
  // Good enough for measuring time because if the other threads are
  // terminated before their pthread_join call - it immediately returns.
  for (unsigned int i = 0; i < MAX_THREAD; i++) {
    assert(pthread_join(thread[i], NULL) == 0);
    //printf("Waiting for thread %u to complete.\n", i);
  }
  printf("Count %u\n", length(&l));
  list_destroy(&l);

  return 0;
}
