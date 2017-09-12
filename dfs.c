#include <stdio.h>
#include "dfs.h"

int checkVert(int** array, int x, int y, int size) {
    if (x != -1 && x != size && y != -1 && y != size) {
        if (array[x][y] == 0) {
            return 1;
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
    if (v.x + 1 == size - 1 && checkVert(array, v.x + 1, v.y, size)) {
        return 1;
    }

    VERT u;

    if (checkVert(array, v.x, v.y + 1, size)) {
        u.x = v.x;
        u.y = v.y + 1;
        push(stack, u);
    }
    if (checkVert(array, v.x, v.y - 1, size)) {
        u.x = v.x;
        u.y = v.y - 1;
        push(stack, u);
    }
    if (checkVert(array, v.x - 1, v.y, size)) {
        u.x = v.x - 1;
        u.y = v.y;
        push(stack, u);
    }
    if (checkVert(array, v.x + 1, v.y, size)) {
        u.x = v.x + 1;
        u.y = v.y;
        push(stack, u);
    }
    return 0;
}

int dfsUpDown(int** array, int size) {
    STACK stack;

    VERT v;
    int* vertices = calloc(size*size, sizeof(int));
    for (int i = 0; i < size; i++) {
        if (array[0][i] == 0 && vertices[i] == 0) {
            initialise(&stack, size * size);
            v.x = 0;
            v.y = i;
            // printf("x:%d y:%d\n", v.x, v.y);
            push(&stack, v);

            while (!isempty(&stack)) {
                v = pop(&stack);
                if (vertices[v.x * size + v.y] != 1) {
                    vertices[v.x * size + v.y] = 1;
                    if(findAdjUD(array, &stack, v, size)) {
                        if (size <= 50) {
                            printf("SUCCEEDED! - Up to Down search\n\n");
                            printArray(vertices, array, size);
                        }
                        return 1;
                    }
                }
            }
        }
    }
    if (size <= 50) {
        printf("FAILED! - Up to Down search\n\n");
        printArray(vertices, array, size);
    }
    return 0;
}

int findAdjLR(int** array, STACK* stack, VERT v, int size) {
    if (v.y + 1 == size - 1 && checkVert(array, v.x, v.y + 1, size)) {
        return 1;
    }
    VERT u;
    if (checkVert(array, v.x - 1, v.y, size)) {
        u.x = v.x - 1;
        u.y = v.y;
        push(stack, u);
    }
    if (checkVert(array, v.x + 1, v.y, size)) {
        u.x = v.x + 1;
        u.y = v.y;
        push(stack, u);
    }
    if (checkVert(array, v.x, v.y - 1, size)) {
        u.x = v.x;
        u.y = v.y - 1;
        push(stack, u);
    }
    if (checkVert(array, v.x, v.y + 1, size)) {
        u.x = v.x;
        u.y = v.y + 1;
        push(stack, u);
    }
    return 0;
}

int dfsLeftRight(int** array, int size) {
    STACK stack;

    VERT v;
    int* vertices = calloc(size*size, sizeof(int));
    for (int i = 0; i < size; i++) {
        if (array[i][0] == 0 && vertices[i * size] == 0) {
            initialise(&stack, size * size);
            v.x = i;
            v.y = 0;
            // printf("x:%d y:%d\n", v.x, v.y);
            push(&stack, v);

            while (!isempty(&stack)) {
                v = pop(&stack);
                if (vertices[v.x * size + v.y] != 1) {
                    vertices[v.x * size + v.y] = 1;
                    if(findAdjLR(array, &stack, v, size)) {
                        if (size <= 50) {
                            printf("SUCCEEDED! - Left to Right search\n\n");
                            printArray(vertices, array, size);
                        }
                        return 1;
                    }
                }
            }
        }
    }
    // clear(&stack);
    if (size <= 50) {
        printf("FAILED! - Left to Right search\n\n");
        printArray(vertices, array, size);
    }
    return 0;
}
