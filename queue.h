#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x;
    int y;
} QUEUE_VERT;

typedef struct {
    QUEUE_VERT* queue;
    unsigned long long front;
    unsigned long long back;
    unsigned long long size;
    unsigned long long maxsize;
} QUEUE;

extern void queue_print(QUEUE* queue, int size);

extern int queue_isempty(QUEUE* queue);

extern int queue_isfull(QUEUE* queue);

extern QUEUE_VERT queue_peek(QUEUE* queue);

extern QUEUE_VERT dequeue(QUEUE* queue);

extern int enqueue(QUEUE* queue, QUEUE_VERT data);

extern int queue_initialise(QUEUE* queue, unsigned long long size);

extern void queue_clear(QUEUE* queue);

void queue_free(QUEUE* queue);

#endif
