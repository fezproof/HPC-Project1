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

int percolate(int** arr, int size) {
    return (dfsUpDown(arr, size) && dfsLeftRight(arr, size));
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
    int size;
    double chance;
    if (argc != 3) {
        size = 50;
        chance = 0.4;
    } else {
        size = atoi(argv[1]);
        chance = atof(argv[2]);
    }

    srand(time(NULL));
    int** lattice = createLattice(size, chance);

    if (size <= 50) {
        // printLattice(lattice, size);
    }

    int test = percolate(lattice, size);

    printf("%s\n", test ? "Can percolate" : "Does not percolate");

    destroyArray(lattice);

}
