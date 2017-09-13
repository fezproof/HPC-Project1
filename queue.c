#include "queue.h"

int queue_isempty(QUEUE* queue) {

   if(queue->size == 0)
      return 1;
   else
      return 0;
}

int queue_isfull(QUEUE* queue) {

   if(queue->size == queue->maxsize)
      return 1;
   else
      return 0;
}

QUEUE_VERT queue_peek(QUEUE* queue) {
   return queue->queue[queue->front];
}

QUEUE_VERT dequeue(QUEUE* queue) {

   if(!queue_isempty(queue)) {
      QUEUE_VERT data = queue->queue[queue->front];
      queue->front = (queue->front - 1 + queue->maxsize) % queue->maxsize;
      queue->size--;
      return data;
   } else {
      printf("Could not retrieve data, Queue is empty.\n");
      exit(0);
   }
}

int enqueue(QUEUE* queue, QUEUE_VERT data) {

   if(!queue_isfull(queue)) {
      queue->queue[queue->back] = data;
      queue->back = (queue->back - 1 + queue->maxsize) % queue->maxsize;
      queue->size++;
      return 1;
   } else {
      printf("Could not insert data, Queue is full.\n");
      return 0;
  }
}

void queue_initialise(QUEUE* queue, int maxsize) {
    queue->queue = calloc(maxsize, sizeof(QUEUE_VERT));
    queue->front = 0;
    queue->back = 0;
    queue->size = 0;
    queue->maxsize = maxsize;
}

void queue_clear(QUEUE* queue) {
    queue->front = 0;
    queue->back = 0;
    queue->size = 0;
}

void queue_free(QUEUE* queue) {
    free(queue->queue);
}
