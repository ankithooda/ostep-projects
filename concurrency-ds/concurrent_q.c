#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

#define MAX_THREADS 8
#define MILLION 1000000

typedef struct __node_t {
  int value;
  struct __node_t *next;
} node_t;

// Queue runs from head->tail.
// Enqueue happens at tail.
// Dequeue happens at head.
typedef struct __queue {
  node_t *head;
  node_t *tail;
  pthread_mutex_t head_lock;
  pthread_mutex_t tail_lock;
} queue_t;

queue_t data;

void queue_init(queue_t *q) {
  q->head = NULL;
  q->tail = NULL;
  assert(pthread_mutex_init(&q->head_lock, NULL) == 0);
  assert(pthread_mutex_init(&q->tail_lock, NULL) == 0);
}

// enqueue adds an element at the end of the list.
int enqueue(queue_t *q, int value) {
  node_t *new = malloc(sizeof(node_t));

  if (new == NULL) {
    perror("Could not allocate memory\n");
    return -1;
  } else {
    new->value = value;
    new->next = NULL;

    assert(pthread_mutex_lock(&q->tail_lock) == 0);
    if (q->tail == NULL) {
      q->tail = new;
      // Now we have to take another lock for the head.
      assert(pthread_mutex_lock(&q->head_lock) == 0);
      q->head = new;
      assert(pthread_mutex_unlock(&q->head_lock) == 0);
    } else {
      q->tail->next = new;
      q->tail = new;
    }
    assert(pthread_mutex_unlock(&q->tail_lock) == 0);
    return 0;
  }
}

node_t *dequeue(queue_t *q) {
  node_t *n;
  assert(pthread_mutex_lock(&q->head_lock) == 0);
  if (q->head == NULL) {
    assert(pthread_mutex_unlock(&q->head_lock) == 0);
    return NULL;
  } else {

    n = q->head;
    q->head = q->head->next;

    // If there was one element in the list
    if (q->head == NULL) {
      assert(pthread_mutex_lock(&q->tail_lock) == 0);
      q->tail = NULL;
      assert(pthread_mutex_unlock(&q->tail_lock) == 0);
    }
  }
  assert(pthread_mutex_unlock(&q->head_lock) == 0);
  return n;
}

void *task(void *arg) {
  unsigned int id = *(unsigned int *)arg;
  printf("Started thread: %u\n", id);
  for (int i = 0; i < MILLION; i++) {
    if (i % 2 == 0) {
      enqueue(&data, i);
    } else {
      dequeue(&data);
    }
  }
  return NULL;
}

int main() {
  pthread_t threads[MAX_THREADS];
  unsigned int thread_ids[MAX_THREADS];
  queue_init(&data);

  for (int i = 0; i < MAX_THREADS; i++) {
    thread_ids[i] = i;
    assert(pthread_create(&threads[i], NULL, task, &thread_ids[i]) == 0);
  }

  for (int i = 0; i < MAX_THREADS; i++) {
    assert(pthread_join(threads[i], NULL) == 0);
  }

  printf("All threads done\n");
  return 0;
}
