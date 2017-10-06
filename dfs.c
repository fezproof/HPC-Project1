#include "dfs.h"

#define PRINT 0

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
    if (checkVertUDSite(array, v.x - 1, v.y, &u, size)) {
        stack_push(stack, u);
    }
    if (checkVertUDSite(array, v.x, v.y + 1, &u, size)) {
        stack_push(stack, u);
    }
    if (checkVertUDSite(array, v.x, v.y - 1, &u, size)) {
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

    unsigned long long numSites = (unsigned long long) size;
    numSites = numSites * numSites;

    char** vertices = malloc(size * sizeof(char*));
    for (int i = 0; i < size; i++) {
        char* values;
        values = calloc(size, sizeof(char));
        vertices[i] = values;
    }

    for (int i = 0; i < size; i++) {
        if (array[0][i] == 1 && vertices[0][i] == 0) {
            stack_initialise(&stack, numSites);

            v.x = 0;
            v.y = i;
            stack_push(&stack, v);

            while (!stack_isempty(&stack)) {

                v = stack_pop(&stack);

                if (vertices[v.x][v.y] != 1) {
                    vertices[v.x][v.y] = 1;
                    if(findAdjUDSite(array, &stack, v, size)) {
                        if (PRINT && size <= 128) {
                            printLatticeSite(array, size, vertices);
                        }
                        stack_clear(&stack);
                        for (int i = 0; i < size; i++) {
                            free(vertices[i]);
                        }
                        free(vertices);

                        return 1;
                    }
                }

            }
            // if (!stack_isempty(&stack))
                stack_clear(&stack);
        }
    }
    if (PRINT && size <= 128) {
        printLatticeSite(array, size, vertices);
    }
    // stack_clear(&stack);

    for (int i = 0; i < size; i++) {
        free(vertices[i]);
    }
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
    if (checkVertLRSite(array, v.x, v.y - 1, &u, size)) {
        stack_push(stack, u);
    }
    if (checkVertLRSite(array, v.x - 1, v.y, &u, size)) {
        stack_push(stack, u);
    }
    if (checkVertLRSite(array, v.x + 1, v.y, &u, size)) {
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

    unsigned long long numSites = (unsigned long long) size;
    numSites = numSites * numSites;

    char** vertices = malloc(size * sizeof(char*));
    for (int i = 0; i < size; i++) {
        char* values;
        values = calloc(size, sizeof(char));
        vertices[i] = values;
    }

    for (int i = 0; i < size; i++) {
        if (array[i][0] == 1 && vertices[i][0] == 0) {
            stack_initialise(&stack, numSites);
            v.x = i;
            v.y = 0;
            stack_push(&stack, v);

            while (!stack_isempty(&stack)) {
                v = stack_pop(&stack);

                if (vertices[v.x][v.y] != 1) {
                    vertices[v.x][v.y] = 1;
                    if(findAdjLRSite(array, &stack, v, size)) {
                        if (PRINT && size <= 128) {
                            printLatticeSite(array, size, vertices);
                        }
                        stack_clear(&stack);
                        for (int i = 0; i < size; i++) {
                            free(vertices[i]);
                        }
                        free(vertices);

                        return 1;
                    }
                }
            }
            // if (!stack_isempty(&stack))
                stack_clear(&stack);
        }
    }
    if (PRINT && size <= 128) {
        printLatticeSite(array, size, vertices);
    }
    // stack_clear(&stack);

    for (int i = 0; i < size; i++) {
        free(vertices[i]);
    }
    free(vertices);

    return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int checkVertBond(BONDSITE** array, int x, int y) {
    return (array[x][y].up || array[x][y].down || array[x][y].left || array[x][y].right);
}

int findAdjUDBond(BONDSITE** array, STACK* stack, VERT v, int size) {
    if (array[v.x][v.y].down && v.x + 1 == size - 1 && array[v.x + 1][v.y].down) {
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

int dfsUpDownBond(BONDSITE** array, int size) {
    STACK stack;

    VERT v;

    unsigned long long numSites = (unsigned long long) size;
    numSites = numSites * numSites;

    char** vertices = malloc(size * sizeof(char*));
    for (int i = 0; i < size; i++) {
        char* values;
        values = calloc(size, sizeof(char));
        vertices[i] = values;
    }

    for (int i = 0; i < size; i++) {
        if (checkVertBond(array, 0, i) && vertices[0][i] == 0) {
            stack_initialise(&stack, numSites);
            v.x = 0;
            v.y = i;
            stack_push(&stack, v);

            while (!stack_isempty(&stack)) {
                v = stack_pop(&stack);

                if (vertices[v.x][v.y] != 1) {
                    vertices[v.x][v.y] = 1;
                    if(findAdjUDBond(array, &stack, v, size)) {
                        if (PRINT && size <= 32) {
                            printLatticeBond(array, size, vertices);
                        }
                        stack_clear(&stack);
                        for (int i = 0; i < size; i++) {
                            free(vertices[i]);
                        }
                        free(vertices);

                        return 1;
                    }
                }
            }
            stack_clear(&stack);
        }
    }
    if (PRINT && size <= 32) {
        printLatticeBond(array, size, vertices);
    }
    for (int i = 0; i < size; i++) {
        free(vertices[i]);
    }
    free(vertices);

    return 0;
}

int findAdjLRBond(BONDSITE** array, STACK* stack, VERT v, int size) {
    if (array[v.x][v.y].right && v.y + 1 == size - 1 && array[v.x][v.y + 1].right) {
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

int dfsLeftRightBond(BONDSITE** array, int size) {
    STACK stack;

    VERT v;

    unsigned long long numSites = (unsigned long long) size;
    numSites = numSites * numSites;

    char** vertices = malloc(size * sizeof(char*));
    for (int i = 0; i < size; i++) {
        char* values;
        values = calloc(size, sizeof(char));
        vertices[i] = values;
    }

    for (int i = 0; i < size; i++) {
        if (checkVertBond(array, i, 0) && vertices[i][0] == 0) {
            stack_initialise(&stack, numSites);
            v.x = i;
            v.y = 0;
            stack_push(&stack, v);

            while (!stack_isempty(&stack)) {
                v = stack_pop(&stack);

                if (vertices[v.x][v.y] != 1) {
                    vertices[v.x][v.y] = 1;
                    if(findAdjLRBond(array, &stack, v, size)) {
                        if (PRINT && size <= 32) {
                            printLatticeBond(array, size, vertices);
                        }
                        stack_clear(&stack);
                        for (int i = 0; i < size; i++) {
                            free(vertices[i]);
                        }
                        free(vertices);

                        return 1;
                    }
                }
            }
            stack_clear(&stack);
        }
    }
    if (PRINT && size <= 32) {
        printLatticeBond(array, size, vertices);
    }

    for (int i = 0; i < size; i++) {
        free(vertices[i]);
    }
    free(vertices);

    return 0;
}
