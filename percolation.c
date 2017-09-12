#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

double rand_01(void) {
/*random number between 0 and 1*/
    return (double)rand() / (double)RAND_MAX;
}

bool** createLattice(int size, double chance)
{
    bool* values;
    bool** rows = malloc(size*sizeof(bool*));
    for (int i = 0; i < size; i++) {
        values = calloc(size*size, sizeof(bool));
        for (int j = 0; j < size; j++) {
            if (chance >= rand_01()) {
                values[j] = true;
            } else {
                values[j] = false;
            }
        }
        rows[i] = values;
    }
    return rows;
}

bool percolate(bool** arr) {


    return true;
}

void destroyArray(bool** arr)
{
    free(*arr);
    free(arr);
}

int main(int argc, char *argv[])
{
    int size;
    double chance;
    if (argc != 3) {
        size = 256;
        chance = 0.5;
    } else {
        size = atoi(argv[1]);
        chance = atof(argv[2]);
    }

    srand(time(NULL));
    bool** lattice = createLattice(size, chance);

    // for (int i = 0; i < size; i++) {
    //     for (int j = 0; j < size; j++) {
    //         printf("%s ", lattice[i][j] ? "X" : "O");
    //     }
    //     printf("\n");
    // }

    bool test = percolate(lattice);

    printf("%s\n", test ? "true" : "false");

    destroyArray(lattice);

}
