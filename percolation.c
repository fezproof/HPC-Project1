#include "percolation.h"

double rand_01(void) {
/*random number between 0 and 1*/
    return (double)rand() / (double)RAND_MAX;
}

char** createLattice(int size, double chance)
{
    char* values;
    char** rows = malloc(size * sizeof(char*));
    for (int i = 0; i < size; i++) {
        values = calloc(size, sizeof(char));
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

int percolateSite(char** arr, int size, int type) {
    if (type == 0) {
        return dfsUpDown(arr, size);
    } else if (type == 1) {
        return dfsLeftRight(arr, size);
    } else if (type == 3) {
        return (dfsUpDown(arr, size) && dfsLeftRight(arr, size));
    }
    return 0;
}

void destroyArray(char** arr)
{
    free(*arr);
    free(arr);
}

void printLattice(char** lattice, int size)
{
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if(lattice[i][j] == (char) 0) printf(". ");
            else if(lattice[i][j] == (char) 1) printf("X ");
            else printf("%c ", lattice[i][j]);
        }
        printf("\n");
    }
}

// int main(int argc, char *argv[])
// {
//     srand(time(NULL));
//
//     int size = 65536;
//     float chance = 0.6;
//
//     printf("Working...\n");
//
//
//     char** lattice = createLattice(size, chance);
//     // printLattice(lattice, size);
//     unsigned long long largestClusterSize = findLargestCluster(lattice, size);
//     printf("\n\nLargest cluster: %llu", largestClusterSize);
//     printf("\n");
// }

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
        char** lattice;
        clock_t begin;
        clock_t end;
        unsigned long long largestClusterSize;
        int percResult;

        //Initialise a CSV file
        FILE *fp = fopen("site.csv","a+");

        time_t curtime;
        time(&curtime);
        fprintf(fp, "\n\n\n%s", ctime(&curtime));
        fprintf(fp, "\nType,Site");
        fprintf(fp, "\nProbability,%f", chance);
        fprintf(fp, "\nThreads,%d", 1);

        fprintf(fp,"\nSize,Allocation,Percolation,Cluster,Total,,Peroclates?,Cluster size");

        double allocationTime;
        double percolationTime;
        double clusterTime;

        do {
            printf("\n------------------------------------------\n");
            printf("Lattice size = %d x %d\n", size, size);

            begin = clock();
            lattice = createLattice(size, chance);
            if(lattice == NULL) {
                printf("Failed whilst creating lattice\n");
                break;
            } else {
                end = clock();
                allocationTime = (double)(end-begin) / CLOCKS_PER_SEC;
                printf("Allocation:\n");
                printf("\tTime taken: %f\n", allocationTime);
            }


            begin = clock();
            percResult = percolateSite(lattice, size, test);
            if(percResult == 2) {
                printf("Failed whilst checking for percolation\n");
                break;
            } else {
                end = clock();
                percolationTime = (double)(end-begin) / CLOCKS_PER_SEC;
                printf("Percolation:\n");
                printf("\tTime taken: %f\n", percolationTime);
                printf("\t%s\n", percResult ? "SUCCEEDED" : "FAILED");
            }

            begin = clock();
            if(chance > 0) {
                largestClusterSize = findLargestCluster(lattice, size);
                if(largestClusterSize > 0) {
                    end = clock();
                    clusterTime = (double)(end-begin) / CLOCKS_PER_SEC;
                    printf("Cluster:\n");
                    printf("\tTime taken: %f\n", clusterTime);
                    printf("\tLargest cluster = %lld sites", largestClusterSize);
                } else {
                    printf("Failed whilst finding the largest cluster\n");
                    break;
                }
            } else {
                printf("Cluster:\n");
                printf("\tTime taken: 0\n");
                printf("\tLargest cluster = 0 sites\n");
            }


            destroyArray(lattice);

            //add to csv file
            fprintf(fp,"\n%d,%f,%f,%f,%f,,%s,%llu", size, allocationTime, percolationTime, clusterTime, allocationTime+percolationTime+clusterTime, percResult ? "SUCCEEDED" : "FAILED", largestClusterSize);

            size = size * 2;

        } while (size * sizeof(int) < 140730606792704);

        fclose(fp);

        // while (!percolateSite(lattice, size, test)) {
        //     end = clock();
        //     elapsedTime = (double)(end-begin) / CLOCKS_PER_SEC;
        //     printf("Percolation - Time taken: %f\n", elapsedTime);
        //
        //     begin = clock();
        //
        //     largestClusterSize = findLargestCluster(lattice, size);
        //
        //     end = clock();
        //     elapsedTime = (double)(end-begin) / CLOCKS_PER_SEC;
        //     printf("Largest cluster contains %d sites\n", largestClusterSize);
        //     printf("Cluster - Time taken: %f\n", elapsedTime);
        //
        //     destroyArray(lattice);
        //     size = size * 2;
        //     if (size > 30000) {
        //         size = size / 2;
        //         printf("Hit threshold limit (%d)\n", size);
        //         exit(0);
        //     }
        //     printf("%d\n", size);
        //     lattice = createLattice(size, chance);
        //     begin = clock();
        // }
        // printf("min size was %d\n", size);

        return 0;
    }
}
