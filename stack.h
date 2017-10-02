#include <stdio.h>
#include <stdlib.h>
#include "bond.h"

typedef struct {
    int x;
    int y;
} VERT;

typedef struct {
    VERT* stack;
    int top;
    int maxsize;
} STACK;

extern int stack_isempty(STACK* stack);

extern int stack_isfull(STACK* stack);

extern VERT stack_peek(STACK* stack);

extern VERT stack_pop(STACK* stack);

extern int stack_push(STACK* stack, VERT data);

extern void stack_initialise(STACK* stack, int size);

extern void stack_clear(STACK* stack);
