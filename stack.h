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

typedef struct {
    BONDSITE* stack;
    int top;
    int maxsize;
} BOND_STACK;

extern int stack_isempty(STACK* stack);

extern int stack_isfull(STACK* stack);

extern VERT stack_peek(STACK* stack);

extern VERT stack_pop(STACK* stack);

extern int stack_push(STACK* stack, VERT data);

extern void stack_initialise(STACK* stack, int size);

extern void stack_clear(STACK* stack);

extern int bond_stack_isempty(BOND_STACK* stack);

extern int bond_stack_isfull(BOND_STACK* stack);

extern BONDSITE bond_stack_peek(BOND_STACK* stack);

extern BONDSITE bond_stack_pop(BOND_STACK* stack);

extern int bond_stack_push(BOND_STACK* stack, BONDSITE data);

extern void bond_stack_initialise(BOND_STACK* stack, int size);

extern void bond_stack_clear(BOND_STACK* stack);
