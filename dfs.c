#include <stdio.h>
#include "dfs.h"

int checkVertUD(int** array, int x, int y, VERT * u, int size) {
    if (x != -1 && x != size) {
        if (y != -1 && y != size) {
            if (array[x][y] == 1) {
                u->x = x;
                u->y = y;
                return 1;
            }
        } else if (y == -1) {
            if (array[x][size - 1] == 1) {
                u->x = x;
                u->y = size - 1;
                return 1;
            }
        } else if (y == size) {
            if (array[x][0] == 1) {
                u->x = x;
                u->y = 0;
                return 1;
            }
        }
    }
    return 0;
}

void printArray(int dis[], int** array, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (dis[i * size + j] == 1) {
                printf("\u2588 ");
            } else {
                printf("%s ", array[i][j] ? "X" : ".");
            }
        }
        printf("\n");
    }
    printf("\n");
}

int findAdjUD(int** array, STACK* stack, VERT v, int size) {
    if (v.x + 1 == size - 1 && array[v.x + 1][v.y] == 1) {
        return 1;
    }

    VERT u;

    if (checkVertUD(array, v.x, v.y + 1, &u, size)) {
        stack_push(stack, u);
    }
    if (checkVertUD(array, v.x, v.y - 1, &u, size)) {
        stack_push(stack, u);
    }
    if (checkVertUD(array, v.x - 1, v.y, &u, size)) {
        stack_push(stack, u);
    }
    if (checkVertUD(array, v.x + 1, v.y, &u, size)) {
        stack_push(stack, u);
    }
    return 0;
}

int dfsUpDown(int** array, int size) {
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
                    if(findAdjUD(array, &stack, v, size)) {
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
    // if (size <= 64) {
    //     printf("FAILED! - Up to Down search\n\n");
    //     printArray(vertices, array, size);
    // }
    return 0;
}

int checkVertLR(int** array, int x, int y, VERT * u, int size) {
    if (y != -1 && y != size) {
        if (x != -1 && x != size) {
            if (array[x][y] == 1) {
                u->x = x;
                u->y = y;
                return 1;
            }
        } else if (x == -1) {
            if (array[size - 1][y] == 1) {
                u->x = size - 1;
                u->y = y;
                return 1;
            }
        } else if (x == size) {
            if (array[0][y] == 1) {
                u->x = 0;
                u->y = y;
                return 1;
            }
        }
    }
    return 0;
}

int findAdjLR(int** array, STACK* stack, VERT v, int size) {
    if (v.y + 1 == size - 1 && array[v.x][v.y + 1] == 1) {
        return 1;
    }

    VERT u;
    if (checkVertLR(array, v.x - 1, v.y, &u, size)) {
        stack_push(stack, u);
    }
    if (checkVertLR(array, v.x + 1, v.y, &u, size)) {
        stack_push(stack, u);
    }
    if (checkVertLR(array, v.x, v.y - 1, &u, size)) {
        stack_push(stack, u);
    }
    if (checkVertLR(array, v.x, v.y + 1, &u, size)) {
        stack_push(stack, u);
    }
    return 0;
}

int dfsLeftRight(int** array, int size) {
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
                    if(findAdjLR(array, &stack, v, size)) {
                        stack_clear(&stack);
                        free(vertices);
                        // if (size <= 64) {
                        //     printf("SUCCEEDED! - Left to Right search\n\n");
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
    // if (size <= 64) {
    //     printf("FAILED! - Left to Right search\n\n");
    //     printArray(vertices, array, size);
    // }
    return 0;
}
