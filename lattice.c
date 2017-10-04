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
        values = malloc(size * sizeof(BONDSITE));
        for (int j = 0; j < size; j++) {
            if (chance >= rand_01()) {
                values[j].left = 1; //occupied
                values[(j - 1 + size) % size].right = 1;
            } else {
                values[j].left = 0; //not occupied
            }
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
