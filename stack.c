/* https://www.tutorialspoint.com/data_structures_algorithms/stack_program_in_c.htm */

#include "stack.h"

int isempty(STACK* stack) {

   if(stack->top == -1)
      return 1;
   else
      return 0;
}

int isfull(STACK* stack) {

   if(stack->top == stack->maxsize)
      return 1;
   else
      return 0;
}

VERT peek(STACK* stack) {
   return stack->stack[stack->top];
}

VERT pop(STACK* stack) {

   if(!isempty(stack)) {
      VERT data = stack->stack[stack->top];
      stack->top = stack->top - 1;
      return data;
   } else {
      printf("Could not retrieve data, Stack is empty.\n");
      exit(0);
   }
}

int push(STACK* stack, VERT data) {

   if(!isfull(stack)) {
      stack->top = stack->top + 1;
      stack->stack[stack->top] = data;
      return 1;
   } else {
      printf("Could not insert data, Stack is full.\n");
      return 0;
   }
}

void initialise(STACK* stack, int size) {
    stack->stack = calloc(size, sizeof(VERT));
    stack->maxsize = size;
    stack->top = -1;
}

void clear(STACK* stack) {
    free(stack->stack);
    // free(&stack);
}
