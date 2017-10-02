#include "percolation.h"

double rand_01(void) {
/*random number between 0 and 1*/
    return (double)rand() / (double)RAND_MAX;
}

char** createLatticeSite(int size, double chance)
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

BONDSITE** createLatticeBond(int size, double chance)
{
    BONDSITE* values;
    BONDSITE** rows = malloc(size * sizeof(BONDSITE*));
    for (int i = 0; i < size; i++) {
        values = calloc(size, sizeof(BONDSITE));
        for (int j = 0; j < size; j++) {
            // if (chance >= rand_01()) {
            //     values[j].up = 1; //occupied
            //     rows[(i - 1 + size) % size][j].down = 1;
            // } else {
            //     values[j].up = 0; //not occupied
            // }
            if (chance >= rand_01()) {
                values[j].left = 1; //occupied
                values[(j - 1 + size) % size].right = 1;
            } else {
                values[j].left = 0; //not occupied
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

void destroyArraySite(char** arr)
{
    free(*arr);
    free(arr);
}

void destroyArrayBond(BONDSITE** arr)
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
            lattice = createLatticeSite(size, chance);
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
                    printf("\tLargest cluster = %lld sites\n", largestClusterSize);
                } else {
                    printf("Failed whilst finding the largest cluster\n");
                    break;
                }
            } else {
                printf("Cluster:\n");
                printf("\tTime taken: 0\n");
                printf("\tLargest cluster = 0 sites\n");
            }


            destroyArraySite(lattice);

            //add to csv file
            fprintf(fp,"\n%d,%f,%f,%f,%f,,%s,%llu", size, allocationTime, percolationTime, clusterTime, allocationTime+percolationTime+clusterTime, percResult ? "SUCCEEDED" : "FAILED", largestClusterSize);

            size = size * 2;

        } while (size < 1048576);

        fclose(fp);

        return 1;
    }
    else {
        BONDSITE** lattice;
        clock_t begin;
        clock_t end;
        // double elapsedTime;
        // int largestClusterSize;
        // int percResult;

        //Initialise a CSV file
        FILE *fp = fopen("site.csv","w+");
        fprintf(fp,"Size,Allocation,Percolation,Cluster,Total");

        double allocationTime;
        // double percolationTime;
        // double clusterTime;

        do {
            printf("\n------------------------------------------\n");
            printf("Lattice size = %d x %d\n", size, size);

            begin = clock();
            lattice = createLatticeBond(size, chance);
            end = clock();
            allocationTime = (double)(end-begin) / CLOCKS_PER_SEC;
            printf("Allocation:\n");
            printf("\tTime taken: %f\n", allocationTime);

            // begin = clock();
            // percResult = percolateSite(lattice, size, test);
            // end = clock();
            // percolationTime = (double)(end-begin) / CLOCKS_PER_SEC;
            // printf("Percolation:\n");
            // printf("\tTime taken: %f\n", percolationTime);
            // printf("\t%s\n", percResult ? "SUCCEEDED" : "FAILED");
            //
            // begin = clock();
            // largestClusterSize = findLargestCluster(lattice, size);
            // end = clock();
            // clusterTime = (double)(end-begin) / CLOCKS_PER_SEC;
            // printf("Cluster:\n");
            // printf("\tTime taken: %f\n", clusterTime);
            // printf("\tLargest cluster = %d sites", largestClusterSize);
            //
            destroyArrayBond(lattice);
            //
            // //add to csv file
            // fprintf(fp,"%d,%f,%f,%f,%f", size, allocationTime, percolationTime, clusterTime, allocationTime+percolationTime+clusterTime);

            size = size * 2;

        } while (size < 65536);

        fclose(fp);

        return 1;
    }
}
