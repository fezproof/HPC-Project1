#include "percolation.h"

double rand_01(void) {
/*random number between 0 and 1*/
    return (double)rand() / (double)RAND_MAX;
}

int** createLattice(int size, double chance)
{
    int* values;
    int** rows = malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        values = calloc(size, sizeof(int));
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
            if(lattice[i][j] == 0) printf(". ");
            else if(lattice[i][j] == 1) printf("X ");
            else printf("%d ", lattice[i][j]);
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
        chance = 0.6;
        test = 3;
    } else {
        latticeType = argv[1][0];
        chance = atof(argv[2]);
        test = atoi(argv[3]);
    }
    size = 64;
    srand(time(NULL));
    if (latticeType == 's') {
        int** lattice;
        clock_t begin;
        clock_t end;
        double elapsedTime;
        int largestClusterSize;
        int percResult;

        do {
            printf("Lattice size = %d x %d\n", size, size);

            begin = clock();
            lattice = createLattice(size, chance);
            end = clock();
            elapsedTime = (double)(end-begin) / CLOCKS_PER_SEC;
            printf("Allocation:\n");
            printf("\tTime taken: %f\n", elapsedTime);

            begin = clock();
            percResult = percolateSite(lattice, size, test);
            end = clock();
            elapsedTime = (double)(end-begin) / CLOCKS_PER_SEC;
            printf("Percolation:\n");
            printf("\tTime taken: %f\n", elapsedTime);
            printf("\t%s\n", percResult ? "SUCCEEDED" : "FAILED");

            begin = clock();
            largestClusterSize = findLargestCluster(lattice, size);
            end = clock();
            elapsedTime = (double)(end-begin) / CLOCKS_PER_SEC;
            printf("Cluster:\n");
            printf("\tTime taken: %f\n", elapsedTime);
            printf("\tLargest cluster = %d sites\n\n", largestClusterSize);
            if (size < 128) {
                printLattice(lattice, size);
            }

            destroyArray(lattice);
            size = size * 2;
        } while (size < 30000);

        return 0;
    }
}
