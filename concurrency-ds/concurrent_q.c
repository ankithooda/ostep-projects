#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

#define MAX_THREADS 8
#define MILLION 10

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
  pthread_mutex_init(&q->head_lock, NULL);
  pthread_mutex_init(&q->tail_lock, NULL);
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

    pthread_mutex_lock(&q->tail_lock);
    if (q->tail == NULL) {
      q->tail = new;
      // Now we have to take another lock for the head.
      pthread_mutex_lock(&q->head_lock);
      q->head = new;
      pthread_mutex_unlock(&q->head_lock);
    } else {
      q->tail->next = new;
      q->tail = new;
    }
    pthread_mutex_unlock(&q->tail_lock);
    return 0;
  }
}

node_t *dequeue(queue_t *q) {
  node_t *n;
  pthread_mutex_lock(&q->head_lock);
  if (q->head == NULL) {
    return NULL;
  } else {
    n = q->head;
    q->head = q->head->next;
  }
  pthread_mutex_unlock(&q->head_lock);
  return n;
}

void *task(void *arg) {
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
  queue_init(&data);

  for (int i = 0; i < MAX_THREADS; i++) {
    assert(pthread_create(&threads[i], NULL, task, NULL) == 0);
  }

  for (int i = 0; i < MAX_THREADS; i++) {
    assert(pthread_join(threads[i], NULL) == 0);
  }

  return 0;
}
