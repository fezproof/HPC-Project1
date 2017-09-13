#include "percolation.h"

double rand_01(void) {
/*random number between 0 and 1*/
    return (double)rand() / (double)RAND_MAX;
}

int** createLattice(int size, double chance)
{
    int* values;
    int** rows = malloc(size*sizeof(int*));
    for (int i = 0; i < size; i++) {
        values = calloc(size*size, sizeof(int));
        for (int j = 0; j < size; j++) {
            if (chance >= rand_01()) {
                values[j] = 1;
            } else {
                values[j] = 0;
            }
        }
        rows[i] = values;
    }
    return rows;
}

int percolateSite(int** arr, int size, int type) {
    if (type == 0) {
        return dfsUpDown(arr, size);
    } else if (type == 1) {
        return dfsLeftRight(arr, size);
    } else if (type == 3) {
        return (dfsUpDown(arr, size) && dfsLeftRight(arr, size));
    }
    return 0;
}

void destroyArray(int** arr)
{
    free(*arr);
    free(arr);
}

void printLattice(int** lattice, int size)
{
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%s ", lattice[i][j] ? "X" : ".");
        }
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    char latticeType;
    int size;
    double chance;
    int test;
    if (argc != 4) {
        latticeType = 's';
        chance = 0.4;
        test = 3;
    } else {
        latticeType = argv[1][0];
        chance = atof(argv[2]);
        test = atoi(argv[3]);
    }
    size = 64;
    srand(time(NULL));
    if (latticeType == 's') {
        int** lattice = createLattice(size, chance);

        while (!percolateSite(lattice, size, test)) {
            destroyArray(lattice);
            size = size * 2;
            if (size > 30000) {
                size = size / 2;
                printf("Hit threshold limit (%d)\n", size);
                exit(0);
            }
            printf("%d\n", size);
            lattice = createLattice(size, chance);
        }

        printf("min size was %d\n", size);

    }

}
