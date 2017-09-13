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
                values[j] = 1; //occupied
            } else {
                values[j] = 0; //not occupied
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
            if(lattice[i][j] == 0) printf(".");
            else if(lattice[i][j] == 1) printf("X");
            else printf("%d", lattice[i][j]);
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

    // printf("\n\n");
    // printLattice(lattice, size);
    // printf("\n\n");

    clock_t begin = clock();

    int largestClusterSize = findLargestCluster(lattice, size);
    printf("Largest cluster contains %d sites\n", largestClusterSize);

    clock_t end = clock();
    double elapsedTime = (double)(end-begin) / CLOCKS_PER_SEC;
    printf("Time taken: %f\n", elapsedTime);

    // printf("\n\n");
    // printLattice(lattice, size);
    // printf("\n\n");

    // printf("Size of int: %lu\n", sizeof(int));
    // printf("Size of int array element: %lu\n\n", sizeof(lattice[0][0]));
    //
    // printf("Size of int array element pointer: %lu\n", sizeof(&lattice[0][0]));
    // printf("Size of VERT struct: %lu\n", sizeof(QUEUE_VERT));

    destroyArray(lattice);

}
