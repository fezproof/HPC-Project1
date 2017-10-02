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

//returns 1 if failed to enqueue item
int enqueue(QUEUE* queue, QUEUE_VERT data) {

   if(!queue_isfull(queue)) {
      queue->queue[queue->back] = data;
      queue->back = (queue->back - 1 + queue->maxsize) % queue->maxsize;
      queue->size++;
      return 0;
   } else {
      printf("Could not insert data, Queue is full.\n");
      return 1;
  }
}

//return 1 if failed to allocate memory
int queue_initialise(QUEUE* queue, unsigned long long maxsize) {
    if(maxsize <= 0) {
        printf("Failed to initialise queue\n");
        return 1;
    }
    queue->queue = calloc(maxsize, sizeof(QUEUE_VERT));
    queue->front = 0;
    queue->back = 0;
    queue->size = 0;
    queue->maxsize = maxsize;
    return 0;
}

void queue_clear(QUEUE* queue) {
    queue->front = 0;
    queue->back = 0;
    queue->size = 0;
}

void queue_free(QUEUE* queue) {
    free(queue->queue);
}
