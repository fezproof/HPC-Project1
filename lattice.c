#include "lattice.h"

double rand_01(void) {
/*random number between 0 and 1*/
    return (double)rand() / (double)RAND_MAX;
}

char** createLatticeSite(int size, double chance)
{
    char** rows = malloc(size * sizeof(char*));
    #pragma omp parallel for
        for (int i = 0; i < size; i++) {
            char* values;
            values = malloc(size * sizeof(char));
            for (int j = 0; j < size; j++) {
                if (chance >= rand_01()) {
                    values[j] = 1; //occupied
                } else {
                    values[j] = 0; //not occupied
                }
            }
            rows[i] = values;
        }
    return rows;
}

BONDSITE** createLatticeBond(int size, double chance)
{
    BONDSITE* values;
    BONDSITE** rows = malloc(size * sizeof(BONDSITE*));
    for (int i = 0; i < size; i++) {
        values = calloc(size, sizeof(BONDSITE));
        for (int j = 0; j < size; j++) {
            if (chance >= rand_01()) {
                values[j].left = 1; //occupied
                values[(j - 1 + size) % size].right = 1;
            } else {
                values[j].left = 0; //not occupied
            }
            values[j].seen = 0;
        }
        rows[i] = values;
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (chance >= rand_01()) {
                rows[i][j].up = 1; //occupied
                rows[(i - 1 + size) % size][j].down = 1;
            } else {
                rows[i][j].up = 0; //not occupied
            }
        }
    }
    return rows;
}

char** copyLatticeSite(char** src, int size)
{
    char** rows = malloc(size * sizeof(char*));
    for (int i = 0; i < size; i++) {
        char* values;
        values = malloc(size * sizeof(char));
        for (int j = 0; j < size; j++) {
            values[j] = src[i][j];
        }
        rows[i] = values;
    }
    return rows;
}

char** copyLatticeSiteThread(char** src, int size)
{
    char** rows = malloc(size * sizeof(char*));
    #pragma omp parallel for
        for (int i = 0; i < size; i++) {
            char* values;
            values = malloc(size * sizeof(char));
            for (int j = 0; j < size; j++) {
                values[j] = src[i][j];
            }
            rows[i] = values;
        }
    return rows;
}

BONDSITE** copyLatticeBond(BONDSITE** src, int size)
{
    BONDSITE** rows = malloc(size * sizeof(BONDSITE*));

    for (int i = 0; i < size; i++) {
        BONDSITE* values;
        values = malloc(size * sizeof(BONDSITE));
        memcpy(values, src[i], size * sizeof(BONDSITE));
        rows[i] = values;
    }
    return rows;
}

BONDSITE** copyLatticeBondThread(BONDSITE** src, int size)
{

    BONDSITE** rows = malloc(size * sizeof(BONDSITE*));

    #pragma omp parallel
    for (int i = 0; i < size; i++) {
        BONDSITE* values;
        values = malloc(size * sizeof(BONDSITE));
        memcpy(values, src[i], size * sizeof(BONDSITE));
        rows[i] = values;
    }
    return rows;
}


void destroyArraySite(char** arr, int size)
{
    for (int i = 0; i < size; i++) {
        free(arr[i]);
    }
    free(arr);
}

void destroyArrayBond(BONDSITE** arr, int size)
{
    for (int i = 0; i < size; i++) {
        free(arr[i]);
    }
    free(arr);
}

void printLatticeSite(char** lattice, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (lattice[i][j] == 2) {
                printf(" \u2588");
            }
            else if (lattice[i][j]) {
                printf(" X");
            }
            else {
                printf("  ");
            }
        }
        printf("\n");
    }
    printf("~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

void printLatticeBond(BONDSITE** lattice, int size)
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
                if (lattice[i][j].seen) {
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
    printf("~~~~~~~~~~~~~~~~~~~~~~~~\n");
}
