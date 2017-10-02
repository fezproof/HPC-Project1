#include <stdio.h>
#include "dfs.h"

int checkVertUDSite(char** array, int x, int y, VERT * u, int size) {
    if (x != -1 && x != size) {
        if (array[x][(y + size) % size] == 1) {
            u->x = x;
            u->y = (y + size) % size;
            return 1;
        }
    }
    return 0;
}

int findAdjUDSite(char** array, STACK* stack, VERT v, int size) {
    if (v.x + 1 == size - 1 && array[v.x + 1][v.y] == 1) {
        return 1;
    }

    VERT u;

    if (checkVertUDSite(array, v.x, v.y + 1, &u, size)) {
        stack_push(stack, u);
    }
    if (checkVertUDSite(array, v.x, v.y - 1, &u, size)) {
        stack_push(stack, u);
    }
    if (checkVertUDSite(array, v.x - 1, v.y, &u, size)) {
        stack_push(stack, u);
    }
    if (checkVertUDSite(array, v.x + 1, v.y, &u, size)) {
        stack_push(stack, u);
    }
    return 0;
}

int dfsUpDownSite(char** array, int size) {
    STACK stack;

    VERT v;
    int* vertices = calloc(size*size, sizeof(int));
    for (int i = 0; i < size; i++) {
        if (array[0][i] == 1 && vertices[i] == 0) {
            stack_initialise(&stack, size * size);
            v.x = 0;
            v.y = i;
            stack_push(&stack, v);

            while (!stack_isempty(&stack)) {
                v = stack_pop(&stack);
                if (vertices[v.x * size + v.y] != 1) {
                    vertices[v.x * size + v.y] = 1;
                    if(findAdjUDSite(array, &stack, v, size)) {
                        stack_clear(&stack);
                        free(vertices);
                        // if (size <= 64) {
                        //     printf("SUCCEEDED! - Up to Down search\n\n");
                        //     printArray(vertices, array, size);
                        // }
                        return 1;
                    }
                }
            }
        }
    }
    stack_clear(&stack);
    free(vertices);

    return 0;
}

int checkVertLRSite(char** array, int x, int y, VERT * u, int size) {
    if (y != -1 && y != size) {
        if (array[(x + size) % size][y]) {
            u->x = (x + size) % size;
            u->y = y;
            return 1;
        }
    }
    return 0;
}

int findAdjLRSite(char** array, STACK* stack, VERT v, int size) {
    if (v.y + 1 == size - 1 && array[v.x][v.y + 1] == 1) {
        return 1;
    }

    VERT u;
    if (checkVertLRSite(array, v.x - 1, v.y, &u, size)) {
        stack_push(stack, u);
    }
    if (checkVertLRSite(array, v.x + 1, v.y, &u, size)) {
        stack_push(stack, u);
    }
    if (checkVertLRSite(array, v.x, v.y - 1, &u, size)) {
        stack_push(stack, u);
    }
    if (checkVertLRSite(array, v.x, v.y + 1, &u, size)) {
        stack_push(stack, u);
    }
    return 0;
}

int dfsLeftRightSite(char** array, int size) {
    STACK stack;

    VERT v;
    int* vertices = calloc(size * size, sizeof(int));
    for (int i = 0; i < size; i++) {
        if (array[i][0] == 1 && vertices[i * size] == 0) {
            stack_initialise(&stack, size * size);
            v.x = i;
            v.y = 0;
            stack_push(&stack, v);

            while (!stack_isempty(&stack)) {
                v = stack_pop(&stack);
                if (vertices[v.x * size + v.y] != 1) {
                    vertices[v.x * size + v.y] = 1;
                    if(findAdjLRSite(array, &stack, v, size)) {
                        stack_clear(&stack);
                        free(vertices);

                        return 1;
                    }
                }
            }
        }
    }
    stack_clear(&stack);
    free(vertices);

    return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int checkVertUDBond(BONDSITE** array, int x, int y, VERT * u, int size) {
    if (x != -1 && x != size) {
        if (array[x][(y + size) % size].up == 1) {
            u->x = x;
            u->y = (y + size) % size;
            return 1;
        }
    }
    return 0;
}

int findAdjUDBond(BONDSITE** array, STACK* stack, VERT v, int size) {
    if (v.x + 1 == size - 1 && array[v.x + 1][v.y].up == 1) {
        return 1;
    }

    VERT u;

    if (checkVertUDBond(array, v.x, v.y + 1, &u, size)) {
        stack_push(stack, u);
    }
    if (checkVertUDBond(array, v.x, v.y - 1, &u, size)) {
        stack_push(stack, u);
    }
    if (checkVertUDBond(array, v.x - 1, v.y, &u, size)) {
        stack_push(stack, u);
    }
    if (checkVertUDBond(array, v.x + 1, v.y, &u, size)) {
        stack_push(stack, u);
    }
    return 0;
}

int dfsUpDownBond(BONDSITE** array, int size) {
    STACK stack;

    VERT v;
    int* vertices = calloc(size*size, sizeof(BONDSITE));
    for (int i = 0; i < size; i++) {
        if (array[0][i].up == 1 && vertices[i] == 0) {
            stack_initialise(&stack, size * size);
            v.x = 0;
            v.y = i;
            stack_push(&stack, v);

            while (!stack_isempty(&stack)) {
                v = stack_pop(&stack);
                if (vertices[v.x * size + v.y] != 1) {
                    vertices[v.x * size + v.y] = 1;
                    if(findAdjUDBond(array, &stack, v, size)) {
                        stack_clear(&stack);
                        free(vertices);

                        return 1;
                    }
                }
            }
        }
    }
    stack_clear(&stack);
    free(vertices);

    return 0;
}

int checkVertLRBond(BONDSITE** array, int x, int y, VERT * u, int size) {
    if (y != -1 && y != size) {
        if (array[(x + size) % size][y].up == 1) {
            u->x = (x + size) % size;
            u->y = y;
            return 1;
        }
    }
    return 0;
}

int findAdjLRBond(BONDSITE** array, STACK* stack, VERT v, int size) {
    if (v.y + 1 == size - 1 && array[v.x][v.y + 1].up == 1) {
        return 1;
    }

    VERT u;
    if (checkVertLRBond(array, v.x - 1, v.y, &u, size)) {
        stack_push(stack, u);
    }
    if (checkVertLRBond(array, v.x + 1, v.y, &u, size)) {
        stack_push(stack, u);
    }
    if (checkVertLRBond(array, v.x, v.y - 1, &u, size)) {
        stack_push(stack, u);
    }
    if (checkVertLRBond(array, v.x, v.y + 1, &u, size)) {
        stack_push(stack, u);
    }
    return 0;
}

int dfsLeftRightBond(BONDSITE** array, int size) {
    STACK stack;

    VERT v;
    int* vertices = calloc(size * size, sizeof(BONDSITE));
    for (int i = 0; i < size; i++) {
        if (array[i][0].up == 1 && vertices[i * size] == 0) {
            stack_initialise(&stack, size * size);
            v.x = i;
            v.y = 0;
            stack_push(&stack, v);

            while (!stack_isempty(&stack)) {
                v = stack_pop(&stack);
                if (vertices[v.x * size + v.y] != 1) {
                    vertices[v.x * size + v.y] = 1;
                    if(findAdjLRBond(array, &stack, v, size)) {
                        stack_clear(&stack);
                        free(vertices);

                        return 1;
                    }
                }
            }
        }
    }
    stack_clear(&stack);
    free(vertices);

    return 0;
}
