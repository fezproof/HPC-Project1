/* https://www.tutorialspoint.com/data_structures_algorithms/stack_program_in_c.htm */

#include "stack.h"

int stack_isempty(STACK* stack) {

   if(stack->top == -1)
      return 1;
   else
      return 0;
}

int stack_isfull(STACK* stack) {

   if(stack->top == stack->maxsize)
      return 1;
   else
      return 0;
}

VERT stack_peek(STACK* stack) {
   return stack->stack[stack->top];
}

VERT stack_pop(STACK* stack) {

   if(!stack_isempty(stack)) {
      VERT data = stack->stack[stack->top];
      stack->top = stack->top - 1;
      return data;
   } else {
      printf("Could not retrieve data, Stack is empty.\n");
      exit(0);
   }
}

int stack_push(STACK* stack, VERT data) {

   if(!stack_isfull(stack)) {
      stack->top = stack->top + 1;
      stack->stack[stack->top] = data;
      return 1;
   } else {
      printf("Could not insert data, Stack is full.\n");
      return 0;
   }
}

void stack_initialise(STACK* stack, int size) {
    stack->stack = calloc(size, sizeof(VERT));
    stack->maxsize = size;
    stack->top = -1;
}

void stack_clear(STACK* stack) {
    free(stack->stack);
    // free(stack);
}
