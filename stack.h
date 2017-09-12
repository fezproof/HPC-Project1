#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x;
    int y;
} VERT;

typedef struct {
    VERT* stack;
    int top;
    int maxsize;
} STACK;

extern int isempty(STACK* stack);

extern int isfull(STACK* stack);

extern VERT peek(STACK* stack);

extern VERT pop(STACK* stack);

extern int push(STACK* stack, VERT data);

extern void initialise(STACK* stack, int size);

extern void clear(STACK* stack);
