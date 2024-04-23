#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct __node_t {
  int key;
  struct __node *next;
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

void queue_init(queue_t *q) {
  q->head = NULL;
  q->tail = NULL;
  pthread_mutex_init(&q->head_lock, NULL);
  pthread_mutex_init(&q->tail_lock, NULL);
}

// enqueue adds an element at the end of the list.
int enqueue(queue_t *q, int key) {
  node_t *new = malloc(sizeof(node_t));

  if (new == NULL) {
    perror("Could not allocate memory\n");
    return -1;
  } else {
    new->key = key;
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
  node_t n;
  pthread_mutex_lock(&q->head_lock);
  if (q->head == NULL) {
    return -1;
  } else {
    n = q->head;
    q->head = q->head->next;
  }
  pthread_mutex_unlock(&q->head_lock);
}

int main() {
  return 0;
}
