#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

#define MAX_THREADS 8
#define MILLION 10000000

typedef struct __node_t {
  int value;
  struct __node_t *next;
} node_t;

typedef struct __queue_t {
  node_t *head, *tail;
  pthread_mutex_t head_lock, tail_lock;
} queue_t;

queue_t data;

void queue_init(queue_t *q) {

  // Initialize locks
  pthread_mutex_init(&q->head_lock, NULL);
  pthread_mutex_init(&q->tail_lock, NULL);

  // Use a NULL node for representing an empty queue
  // instead of both head and tail pointers having NULL value.
  // This avoids taking both head and tail locks
  // when inserting new node in an empty queue.
  // See enqueue implementation.
  node_t *temp = malloc(sizeof(node_t));
  temp->next = NULL;

  q->head = temp;
  q->tail = temp;
}

int enqueue(queue_t *q, int value) {

  node_t *new = malloc(sizeof(node_t));

  assert(new != NULL);

  new->value = value;
  new->next = NULL;

  // Take lock
  assert(pthread_mutex_lock(&q->tail_lock) == 0);

  q->tail->next = new;
  q->tail = new;

  // Release lock
  assert(pthread_mutex_unlock(&q->tail_lock) == 0);
  return 0;
}

int dequeue(queue_t *q, int *v) {
  node_t *head, *dummy_head;
  int ret;

  assert(pthread_mutex_lock(&q->head_lock) == 0);

  // In this implementation, the q->head is
  // either a dummy node or a node which has been dequeued.
  dummy_head = q->head;
  head = q->head->next;  // This is the logical head of the queue.

  if (head == NULL) {
    ret = -1;
  } else {
    *v = head->value;
    free(dummy_head);
    q->head = head;
    ret = 0;
  }
  assert(pthread_mutex_unlock(&q->head_lock) == 0);
  return ret;
}

void print_q(queue_t *q) {
  node_t *t;
  t = q->head;
  while(t) {
    printf("%d\n", t->value);
    t = t->next;
  }
}

void *task(void *arg) {
  unsigned int id = *(unsigned int*)arg;
  int *v = malloc(sizeof(int));
  printf("Started Thread: %u\n", id);
  for (int i = 0; i < MILLION; i++) {
    if (i % 3 != 0) {
      enqueue(&data, i);
      //printf("After enqueue\n");
      //print_q(&data);
    } else {
      dequeue(&data, v);
      //printf("After enqueue\n");
      //print_q(&data);
    }
  }
  free(v);
  return NULL;
}

int main() {
  pthread_t threads[MAX_THREADS];
  unsigned int ids[MAX_THREADS];

  queue_init(&data);

  for (unsigned int i = 0; i < MAX_THREADS; i++) {
    ids[i] = i;
    pthread_create(&threads[i], NULL, task, &ids[i]);
  }

  for (unsigned int i = 0; i < MAX_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("main: Done\n");
  return 0;
}
