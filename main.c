#include "main.h"

#define RUNS 100
#define MAX_NUM_THREADS 4

#define START_SIZE 16 //64
#define MAX_LATTICE_SIZE 512 //131072 16384 8192 4096


FILE* initialiseCSV(char latticeType, double chance, int test, int runs, int maxNumThreads)
{
    char* file = latticeType == 's' ? "site.csv" : "bond.csv";
    FILE *fp = fopen(file,"a+");

    time_t curtime;
    time(&curtime);
    fprintf(fp, "\n\n\n%s", ctime(&curtime));
    fprintf(fp, "\nPercolation,%s", latticeType == 's' ? "Site" : "Bond");
    fprintf(fp, "\nProbability,%f", chance);
    fprintf(fp, "\nPerc type,%d,", test);
    fprintf(fp, "\nRuns,%d", runs);

    // printf("Num threads: %d\n", maxNumThreads);

    fprintf(fp, "\n");

    return fp;
}

void moveRight(int x, FILE *fp)
{
    for(int i = 0; i < x; i++) {
        fprintf(fp,",");
    }
}

void initialiseCSVRound(int size, int maxNumThreads, FILE *fp)
{
    fprintf(fp, "\n\nSize,%dx%d,,",size,size);

    fprintf(fp,"Percolation times,");
    moveRight(maxNumThreads, fp);
    fprintf(fp,"Percolation speedup,");
    moveRight(maxNumThreads+1, fp);
    fprintf(fp,"Cluster times,");
    moveRight(maxNumThreads, fp);
    fprintf(fp,"Cluster speedup,");
    moveRight(maxNumThreads+1, fp);
    fprintf(fp,"P+C times,");
    moveRight(maxNumThreads, fp);
    fprintf(fp,"P+C speedup,");
    moveRight(maxNumThreads+2, fp);
    fprintf(fp,"Times percolated (Validation),");
    moveRight(maxNumThreads, fp);
    fprintf(fp,"Avg cluster size (Validation),");

    fprintf(fp,"\nNodes,Allocation,,");

    for(int i = 1; i <= maxNumThreads; i++) {
        fprintf(fp,"T%d,", i);
    }

    fprintf(fp,",T0,");

    for(int i = 1; i <= maxNumThreads; i++) {
        fprintf(fp,"T%d,", i);
    }

    fprintf(fp,",");

    for(int i = 1; i <= maxNumThreads; i++) {
        fprintf(fp,"T%d,", i);
    }

    fprintf(fp,",T0,");

    for(int i = 1; i <= maxNumThreads; i++) {
        fprintf(fp,"T%d,", i);
    }

    fprintf(fp,",");

    for(int i = 1; i <= maxNumThreads; i++) {
        fprintf(fp,"T%d,", i);
    }

    fprintf(fp,",T0,");

    for(int i = 1; i <= maxNumThreads; i++) {
        fprintf(fp,"T%d,", i);
    }

    fprintf(fp,",,");

    for(int i = 1; i <= maxNumThreads; i++) {
        fprintf(fp,"T%d,", i);
    }

    fprintf(fp,",");

    for(int i = 1; i <= maxNumThreads; i++) {
        fprintf(fp,"T%d,", i);
    }

    fprintf(fp,"\n");
}

void printfCSVLine(int maxNumThreads,
int size, int numNodes, double allocationTime,
double* percTimes, double* clusterTimes,
double* percSpeedUp, double* clusterSpeedUp,
double* totalTimes, double* totalSpeedUp,
int* numPercs, unsigned long long* clusterSizes, FILE *fp)
{
    fprintf(fp, "%d,%f,,",numNodes,allocationTime);

    for(int i = 0; i < maxNumThreads; i++) {
        fprintf(fp,"%f,", percTimes[i]);
    }

    fprintf(fp,",0,");

    for(int i = 0; i < maxNumThreads; i++) {
        fprintf(fp,"%f,", percSpeedUp[i]);
    }

    fprintf(fp,",");

    for(int i = 0; i < maxNumThreads; i++) {
        fprintf(fp,"%f,", clusterTimes[i]);
    }

    fprintf(fp,",0,");

    for(int i = 0; i < maxNumThreads; i++) {
        fprintf(fp,"%f,", clusterSpeedUp[i]);
    }

    fprintf(fp,",");

    for(int i = 0; i < maxNumThreads; i++) {
        fprintf(fp,"%f,", totalTimes[i]);
    }

    fprintf(fp,",0,");

    for(int i = 0; i < maxNumThreads; i++) {
        fprintf(fp,"%f,", totalSpeedUp[i]);
    }

    fprintf(fp,",,");

    for(int i = 0; i < maxNumThreads; i++) {
        fprintf(fp,"%d,", numPercs[i]);
    }

    fprintf(fp,",");

    for(int i = 0; i < maxNumThreads; i++) {
        fprintf(fp,"%llu,", clusterSizes[i]);
    }

    fprintf(fp,"\n");

}

void doSiteTests(int size, double chance, int test, int runs, int maxLatticeSize, int maxNumThreads, int maxNumNodes, int rank, FILE *fp)
{

    char** lattice;

    double allocationTime = 0;

    int percResult = 0;
    int percResultThreaded = 0;
    unsigned long long largestClusterSize = 0;
    unsigned long long largestClusterSizeThreaded = 0;

    double percTimes[maxNumThreads];
    double clusterTimes[maxNumThreads];
    double percSpeedUp[maxNumThreads];
    double clusterSpeedUp[maxNumThreads];
    double totalTimes[maxNumThreads];
    double totalSpeedUp[maxNumThreads];
    int numPercs[maxNumThreads];
    unsigned long long clusterSizes[maxNumThreads];

    do {

        printf("\n------------------------------------------\n");
        printf("Lattice size: %d x %d\n", size, size);



        initialiseCSVRound(size, maxNumThreads, fp);


        allocationTime = timeAllocateSite(&lattice, size, chance);


        // printLatticeSite(lattice, size, size);

        largestClusterSize = findLargestClusterSite(lattice, size);
        printf("Largest (Sequential): %llu\n\n", largestClusterSize);

        int n = maxNumNodes;
        // for(int n = 1; n <= maxNumNodes; n++) {
            printf("Number of nodes = %d\n", n);

            memset(percTimes, 0, sizeof percTimes);
            memset(clusterTimes, 0, sizeof clusterTimes);
            memset(percSpeedUp, 0, sizeof percSpeedUp);
            memset(clusterSpeedUp, 0, sizeof clusterSpeedUp);
            memset(numPercs, 0, sizeof numPercs);
            memset(clusterSizes, 0, sizeof clusterSizes);

            for(int i = 0; i < runs; i++)
            {
                int j = maxNumThreads - 1;
                // for(int j = 0; j < maxNumThreads; j++)
                // {
                    int numThreads;
                    if(j+1 >= size) {
                        numThreads = size;
                    } else {
                        numThreads = j+1;
                    }
                    omp_set_num_threads(numThreads);

                    // percTimes[j] += timeSitePerc(lattice, size, test, &percResultThreaded, n, numThreads);
                    percTimes[j] = 1;
                    clusterTimes[j] += timeSiteCluster(lattice, size, chance, &largestClusterSizeThreaded, n, numThreads);

                    if(largestClusterSize != largestClusterSizeThreaded) {
                        printf("\nERROR: CLUSTER SIZE VARIANCE: %llu, %llu\n", largestClusterSize, largestClusterSizeThreaded);
                    }

                    printf("Largest (Threaded): %llu\n", largestClusterSizeThreaded);


                    if(percResult != percResultThreaded) {
                        printf("\nERROR: PERCOLATION VARIANCE");
                    }

                    if(percResultThreaded == 1) numPercs[j]++;
                    clusterSizes[j] += largestClusterSizeThreaded;
                // }


            }


            for(int i = 0; i < maxNumThreads; i++) {
                totalTimes[i] = percTimes[i] + clusterTimes[i];
            }

            for(int i = 0; i < maxNumThreads; i++) {
                percSpeedUp[i] = percTimes[0] / percTimes[i];
                clusterSpeedUp[i] = clusterTimes[0] / clusterTimes[i];
                totalSpeedUp[i] = totalTimes[0] / totalTimes[i];
            }

            for(int i = 0; i < maxNumThreads; i++) {
                percTimes[i] /= (double) runs;
                clusterTimes[i] /= (double) runs;
                totalTimes[i] /= (double) runs;
                clusterSizes[i] /= (double) runs;
            }



            printfCSVLine(maxNumThreads, size, n, allocationTime, percTimes, clusterTimes, percSpeedUp, clusterSpeedUp, totalTimes, totalSpeedUp, numPercs, clusterSizes, fp);

            allocationTime = 0;

            largestClusterSizeThreaded = 0;



        // }
        destroyArraySite(lattice, size);

        largestClusterSize = 0;
        size = size * 2;
    } while (size <= maxLatticeSize);

}

void doBondTests(int size, double chance, int test, int runs, int maxLatticeSize, int maxNumThreads, int maxNumNodes, int rank, FILE *fp)
{

    BONDSITE** lattice;

    double allocationTime = 0;

    int percResult = 0;
    int percResultThreaded = 0;
    unsigned long long largestClusterSize = 0;
    unsigned long long largestClusterSizeThreaded = 0;

    double percTimes[maxNumThreads];
    double clusterTimes[maxNumThreads];
    double percSpeedUp[maxNumThreads];
    double clusterSpeedUp[maxNumThreads];
    double totalTimes[maxNumThreads];
    double totalSpeedUp[maxNumThreads];
    int numPercs[maxNumThreads];
    unsigned long long clusterSizes[maxNumThreads];

    do {

        printf("\n------------------------------------------\n");
        printf("Lattice size = %d x %d\n", size, size);
        initialiseCSVRound(size, maxNumThreads, fp);

        for(int n = 1; n <= maxNumNodes; n++) {

            memset(percTimes, 0, sizeof percTimes);
            memset(clusterTimes, 0, sizeof clusterTimes);
            memset(percSpeedUp, 0, sizeof percSpeedUp);
            memset(clusterSpeedUp, 0, sizeof clusterSpeedUp);
            memset(numPercs, 0, sizeof numPercs);
            memset(clusterSizes, 0, sizeof clusterSizes);

            for(int i = 0; i < runs; i++)
            {
                allocationTime += timeAllocateBond(&lattice, size, chance);
                percTimes[0] += timePercBond(lattice, size, test, &percResult);
                clusterTimes[0] += timeClusterBond(lattice, size, chance, &largestClusterSize);

                if(percResult == 1) numPercs[0]++;
                clusterSizes[0] += largestClusterSize;

                for(int j = 1; j < maxNumThreads; j++)
                {
                    int numThreads;
                    if(j+1 > size) {
                        numThreads = size;
                    } else {
                        numThreads = j + 1;
                    }
                    omp_set_num_threads(numThreads);

                    percTimes[j] += timePercBondThreaded(lattice, size, test, &percResultThreaded);
                    clusterTimes[j] += timeClusterBondThreaded(lattice, size, chance, &largestClusterSizeThreaded, numThreads);

                    if(largestClusterSize != largestClusterSizeThreaded) {
                        printf("\nERROR: CLUSTER SIZE VARIANCE: %llu, %llu\n", largestClusterSize, largestClusterSizeThreaded);
                    }

                    if(percResult != percResultThreaded) {
                        printf("\nERROR: PERCOLATION VARIANCE");
                    }

                    if(percResultThreaded == 1) numPercs[j]++;
                    clusterSizes[j] += largestClusterSizeThreaded;

                }
                destroyArrayBond(lattice, size);
            }


            for(int i = 0; i < maxNumThreads; i++) {
                totalTimes[i] = percTimes[i] + clusterTimes[i];
            }

            for(int i = 0; i < maxNumThreads; i++) {
                percSpeedUp[i] = percTimes[0] / percTimes[i];
                clusterSpeedUp[i] = clusterTimes[0] / clusterTimes[i];
                totalSpeedUp[i] = totalTimes[0] / totalTimes[i];
            }

            allocationTime /= runs;

            for(int i = 0; i < maxNumThreads; i++) {
                percTimes[i] /= (double) runs;
                clusterTimes[i] /= (double) runs;
                totalTimes[i] /= (double) runs;
                clusterSizes[i] /= (double) runs;
            }

            printfCSVLine(maxNumThreads, size, n, allocationTime, percTimes, clusterTimes, percSpeedUp, clusterSpeedUp, totalTimes, totalSpeedUp, numPercs, clusterSizes, fp);

            allocationTime = 0;

            // largestClusterSize = 0;
            largestClusterSizeThreaded = 0;

        }
        size = size * 2;
    } while (size <= maxLatticeSize);
}

int main(int argc, char *argv[])
{
    // initialise MPI
    int numProcs, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    //Prevents the system from changing the number of threads
    omp_set_dynamic(0);

    OPTIONS *options = createOptionsStruct();
    readOptions(argc, argv, options);
    // printOptions(options);

    // if(options->nodeNum > numProcs) {
    //     fprintf(stderr, "Cannot execute with %d nodes. Increase the number of nodes specified in run.sh from %d to %d.", options->nodeNum, options->nodeNum, numProcs);
    //     exit(EXIT_FAILURE);
    // }

    srand(time(NULL));

    if(rank == 0) {
        // int x = 111;
        // for(int n = 0; n < 5; n++) {
        //     x += 100;
        //     for(int i = 1; i < numProcs; i++) {
        //         MPI_Send(&x, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        //     }
        // }
        //
        // for(int i = 1; i < numProcs; i++) {
        //     MPI_Send(NULL, 0, MPI_INT, i, 1, MPI_COMM_WORLD);
        // }

        // Initialise a CSV file
        FILE *fp = initialiseCSV(options->type, options->probability, options->perlocationType, options->runs, options->threadNum);

        if (options->type == 's') {
            doSiteTests(options->minSize, options->probability, options->perlocationType, options->runs, options->maxSize, options->threadNum, options->nodeNum, rank, fp);
        }
        // else if (options->type == 'b') {
        //     doBondTests(options->minSize, options->probability, options->perlocationType, options->runs, options->maxSize, options->threadNum, options->nodeNum, rank, fp);
        // }



        fclose(fp);

        terminateSlaves(numProcs);


    } else {
        // MPI_Status status;
        // int y = 0;
        // while(1) {
        //     MPI_Recv(&y, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        //     if(status.MPI_TAG == 1) {
        //         printf("Slave %d is terminating\n", rank);
        //         break;
        //     }
        //     printf("Slave %d received %d\n", rank, y);
        // }

        if(options->type == 's') {
            clusterSiteSlave();
        }
        // else if (options->type == 'b') {
        //     clusterBondSlave();
        // }
    }


    MPI_Finalize();
}
