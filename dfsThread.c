#include "dfs.h"

void printLatticeBondThread(BONDSITE** lattice, int* vertices, int size)
{
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (lattice[i][j].up == 1) {
                printf(" | ");
            } else {
                printf("   ");
            }
        }
        printf("\n");
        for (int j = 0; j < size; j++) {
            if (lattice[i][j].left == 1) {
                printf("-");
            } else {
                printf(" ");
            }
            if (lattice[i][j].left == 1 || lattice[i][j].right == 1 || lattice[i][j].up == 1 || lattice[i][j].down == 1) {
                if (vertices[i * size + j]) {
                    printf("\u2588");
                } else {
                    printf("+");
                }
            } else {
                printf(" ");
            }
            if (lattice[i][j].right == 1) {
                printf("-");
            } else {
                printf(" ");
            }
        }
        printf("\n");
        // for (int j = 0; j < size; j++) {
        //     if (lattice[i][j].down == 1) {
        //         printf(" | ");
        //     } else {
        //         printf("   ");
        //     }
        // }
        // printf("\n");
    }
}

int checkVertUDSiteThread(char** array, int x, int y, VERT * u, int size) {
    if (x != -1 && x != size) {
        if (array[x][(y + size) % size] == 1) {
            u->x = x;
            u->y = (y + size) % size;
            return 1;
        }
    }
    return 0;
}

int findAdjUDSiteThread(char** array, STACK* stack, VERT v, int size) {
    if (v.x + 1 == size - 1 && array[v.x + 1][v.y] == 1) {
        return 1;
    }

    VERT u;

    if (checkVertUDSiteThread(array, v.x, v.y + 1, &u, size)) {
        stack_push(stack, u);
    }
    if (checkVertUDSiteThread(array, v.x, v.y - 1, &u, size)) {
        stack_push(stack, u);
    }
    if (checkVertUDSiteThread(array, v.x - 1, v.y, &u, size)) {
        stack_push(stack, u);
    }
    if (checkVertUDSiteThread(array, v.x + 1, v.y, &u, size)) {
        stack_push(stack, u);
    }
    return 0;
}

int dfsUpDownSiteThread(char** array, int size) {

    unsigned long long numSites = (unsigned long long) size;
    numSites = numSites * numSites;

    char** vertices = malloc(size * sizeof(char*));
    #pragma omp parallel for
        for (int i = 0; i < size; i++) {
            char* values;
            values = calloc(size, sizeof(char));
            vertices[i] = values;
        }

    char complete = 0;

    #pragma omp parallel for
        for (int i = 0; i < size; i++) {
            if (!complete) {
                if (array[0][i] == 1 && vertices[0][i] == 0) {
                    STACK stack;
                    VERT v;

                    stack_initialise(&stack, numSites);

                    v.x = 0;
                    v.y = i;
                    stack_push(&stack, v);

                    while (!complete && !stack_isempty(&stack)) {

                        v = stack_pop(&stack);

                        if (vertices[v.x][v.y] != 1) {
                            vertices[v.x][v.y] = 1;
                            if(findAdjUDSiteThread(array, &stack, v, size)) {
                                // if (size <= 64) {
                                //     printf("SUCCEEDED! - Up to Down search\n\n");
                                //     printArray(vertices, array, size);
                                // }
                                complete = 1;
                            }
                        }

                    }
                    if (!stack_isempty(&stack))
                        stack_clear(&stack);
                }
            }
        }

    free(*vertices);
    free(vertices);

    return complete;
}

int checkVertLRSiteThread(char** array, int x, int y, VERT * u, int size) {
    if (y != -1 && y != size) {
        if (array[(x + size) % size][y]) {
            u->x = (x + size) % size;
            u->y = y;
            return 1;
        }
    }
    return 0;
}

int findAdjLRSiteThread(char** array, STACK* stack, VERT v, int size) {
    if (v.y + 1 == size - 1 && array[v.x][v.y + 1] == 1) {
        return 1;
    }

    VERT u;
    if (checkVertLRSiteThread(array, v.x - 1, v.y, &u, size)) {
        stack_push(stack, u);
    }
    if (checkVertLRSiteThread(array, v.x + 1, v.y, &u, size)) {
        stack_push(stack, u);
    }
    if (checkVertLRSiteThread(array, v.x, v.y - 1, &u, size)) {
        stack_push(stack, u);
    }
    if (checkVertLRSiteThread(array, v.x, v.y + 1, &u, size)) {
        stack_push(stack, u);
    }
    return 0;
}

int dfsLeftRightSiteThread(char** array, int size) {
    unsigned long long numSites = (unsigned long long) size;
    numSites = numSites * numSites;

    char** vertices = malloc(size * sizeof(char*));
    #pragma omp parallel for
        for (int i = 0; i < size; i++) {
            char* values;
            values = calloc(size, sizeof(char));
            vertices[i] = values;
        }

    char complete = 0;

    #pragma omp parallel for
        for (int i = 0; i < size; i++) {
            if (!complete) {
                if (array[i][0] == 1 && vertices[i][0] == 0) {
                    STACK stack;
                    VERT v;

                    stack_initialise(&stack, numSites);

                    v.x = 0;
                    v.y = i;
                    stack_push(&stack, v);

                    while (!complete && !stack_isempty(&stack)) {

                        v = stack_pop(&stack);

                        if (vertices[v.x][v.y] != 1) {
                            vertices[v.x][v.y] = 1;
                            if(findAdjUDSiteThread(array, &stack, v, size)) {
                                // stack_clear(&stack);
                                // free(vertices);
                                // if (size <= 64) {
                                //     printf("SUCCEEDED! - Up to Down search\n\n");
                                //     printArray(vertices, array, size);
                                // }
                                complete = 1;
                            }
                        }

                    }
                    if (!stack_isempty(&stack))
                        stack_clear(&stack);
                }
            }
        }


    free(*vertices);
    free(vertices);

    return complete;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int checkVertBondThread(BONDSITE** array, int x, int y) {
    return (array[x][y].up || array[x][y].down || array[x][y].left || array[x][y].right);
}

int findAdjUDBondThread(BONDSITE** array, STACK* stack, VERT v, int size) {
    if (v.x + 1 == size - 1 && array[v.x + 1][v.y].down) {
        return 1;
    }

    VERT u;
    if (v.x > -1 && v.x < size) {
        if (v.x > 0 && array[v.x][v.y].up == 1) {
            u.x = (v.x - 1 + size) % size;
            u.y = v.y;
            stack_push(stack, u);
        }
        if (array[v.x][v.y].left == 1) {
            u.x = v.x;
            u.y = (v.y - 1 + size) % size;
            stack_push(stack, u);
        }
        if (array[v.x][v.y].right == 1) {
            u.x = v.x;
            u.y = (v.y + 1 + size) % size;
            stack_push(stack, u);
        }
        if (array[v.x][v.y].down == 1) {
            u.x = (v.x + 1 + size) % size;
            u.y = v.y;
            stack_push(stack, u);
        }
    }
    return 0;
}

int dfsUpDownBondThread(BONDSITE** array, int size) {
    STACK stack;

    VERT v;
    unsigned long long index;
    unsigned long long numSites = (unsigned long long) size;
    numSites = numSites * numSites;
    int* vertices = calloc(numSites, sizeof(BONDSITE));
    for (int i = 0; i < size; i++) {
        if (checkVertBondThread(array, 0, i) && vertices[i] == 0) {
            stack_initialise(&stack, numSites);
            v.x = 0;
            v.y = i;
            stack_push(&stack, v);

            while (!stack_isempty(&stack)) {
                v = stack_pop(&stack);
                index = (unsigned long long) v.x * (unsigned long long) size + (unsigned long long) v.y;

                if (vertices[index] != 1) {
                    vertices[index] = 1;
                    if(findAdjUDBondThread(array, &stack, v, size)) {
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

int findAdjLRBondThread(BONDSITE** array, STACK* stack, VERT v, int size) {
    if (v.y + 1 == size - 1 && array[v.x][v.y + 1].right) {
        return 1;
    }

    VERT u;
    if (v.y > -1 && v.y < size) {
        if (v.y > 0 && array[v.x][v.y].left == 1) {
            u.x = v.x;
            u.y = (v.y - 1 + size) % size;
            stack_push(stack, u);
        }
        if (array[v.x][v.y].up == 1) {
            u.x = (v.x - 1 + size) % size;
            u.y = v.y;
            stack_push(stack, u);
        }
        if (array[v.x][v.y].down == 1) {
            u.x = (v.x + 1 + size) % size;
            u.y = v.y;
            stack_push(stack, u);
        }
        if (array[v.x][v.y].right == 1) {
            u.x = v.x;
            u.y = (v.y + 1 + size) % size;
            stack_push(stack, u);
        }
    }
    return 0;
}

int dfsLeftRightBondThread(BONDSITE** array, int size) {
    STACK stack;

    VERT v;
    unsigned long long index;
    unsigned long long numSites = (unsigned long long) size;
    numSites = numSites * numSites;
    int* vertices = calloc(numSites, sizeof(BONDSITE));
    for (int i = 0; i < size; i++) {
        if (checkVertBondThread(array, i, 0) && vertices[i * size] == 0) {
            stack_initialise(&stack, numSites);
            v.x = i;
            v.y = 0;
            stack_push(&stack, v);

            while (!stack_isempty(&stack)) {
                v = stack_pop(&stack);
                index = (unsigned long long) v.x * (unsigned long long) size + (unsigned long long) v.y;

                if (vertices[index] != 1) {
                    vertices[index] = 1;
                    if(findAdjLRBondThread(array, &stack, v, size)) {
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
