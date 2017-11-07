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
    fprintf(fp,"Execution times,");
    moveRight(maxNumThreads, fp);
    fprintf(fp,"Speedup,");
    moveRight(maxNumThreads+1, fp);
    fprintf(fp,"Times percolated (Validation),");
    moveRight(maxNumThreads+2, fp);
    fprintf(fp,"Avg cluster size (Validation),");
    fprintf(fp,"\nProcessors,Allocation,,");

    for(int i = 1; i <= maxNumThreads; i++) {
        fprintf(fp,"T%d,", i);
    }

    fprintf(fp,",T0,");

    for(int i = 1; i <= maxNumThreads; i++) {
        fprintf(fp,"T%d,", i);
    }

    fprintf(fp,",");
    fprintf(fp,"Seq");
    fprintf(fp,",");

    for(int i = 1; i <= maxNumThreads; i++) {
        fprintf(fp,"T%d,", i);
    }

    fprintf(fp,",,");
    fprintf(fp,"Seq");
    fprintf(fp,",");

    for(int i = 1; i <= maxNumThreads; i++) {
        fprintf(fp,"T%d,", i);
    }

    fprintf(fp,"\n");
}

void printfCSVLine(int maxNumThreads,
int size, int numNodes, double allocationTime,
double* times, double* speedup, int percResultSeq,
int* numPercs, unsigned long long clusterSizeSeq,
unsigned long long* clusterSizes, FILE *fp)
{
    fprintf(fp, "%d,%f,,",numNodes,allocationTime);

    for(int i = 0; i < maxNumThreads; i++) {
        fprintf(fp,"%f,", times[i]);
    }

    fprintf(fp,",0,");

    for(int i = 0; i < maxNumThreads; i++) {
        fprintf(fp,"%f,", speedup[i]);
    }

    fprintf(fp,",");

    fprintf(fp,"%d,", percResultSeq);

    for(int i = 0; i < maxNumThreads; i++) {
        fprintf(fp,"%d,", numPercs[i]);
    }

    fprintf(fp,",,");

    fprintf(fp,"%llu,", clusterSizeSeq);

    for(int i = 0; i < maxNumThreads; i++) {
        fprintf(fp,"%llu,", clusterSizes[i]);
    }

    fprintf(fp,"\n");

}

void printFile(FILE* fp)
{
    printf("\n\n\n\n");
    char line[BUFSIZ];
    while(fgets(line, BUFSIZ, fp) != NULL) {
        printf("%s", line);
    }
    printf("\n\n\n\n");
}

void doTests(char latticeType, int size, double chance, int test, int runs, int maxLatticeSize, int minNumThreads, int maxNumThreads, int minNumNodes, int maxNumNodes, int rank, FILE *fp)
{
    void** lattice;

    double allocationTime = 0;

    int percResultSeq = 0;
    int percResultThreaded = 0;
    unsigned long long largestClusterSizeSeq = 0;
    unsigned long long largestClusterSizeThreaded = 0;

    double times[maxNumThreads];
    double speedup[maxNumThreads];
    int numPercs[maxNumThreads];
    unsigned long long clusterSizes[maxNumThreads];

    do {
        printf("\n------------------------------------------\n");
        printf("Lattice size: %d x %d\n", size, size);

        initialiseCSVRound(size, maxNumThreads, fp);

        allocationTime = timeAllocation(&lattice, latticeType, size, chance);

        // if(latticeType == 's') printLatticeSite((char**)lattice, size, size);
        // else printLatticeBond((BONDSITE**)lattice, size, size);

        for(int n = minNumNodes; n <= maxNumNodes; n++) {

            if(latticeType == 's') {
                largestClusterSizeSeq = findLargestClusterSite((char**)lattice, size);
                percResultSeq = percolateSite((char**)lattice, size, test);
            } else {
                largestClusterSizeSeq = findLargestClusterBond((BONDSITE**)lattice, size);
                percResultSeq = percolateBond((BONDSITE**)lattice, size, test);
            }
            printf("Largest (Sequential): %llu\n", largestClusterSizeSeq);
            printf("Percolated? (Sequential): %s\n\n", percResultSeq == 1 ? "YES" : "NO");

            printf("Number of nodes = %d\n", n);

            memset(times, 0, sizeof times);
            memset(speedup, 0, sizeof speedup);
            memset(numPercs, 0, sizeof numPercs);
            memset(clusterSizes, 0, sizeof clusterSizes);

            for(int i = 0; i < runs; i++)
            {
                for(int j = minNumThreads; j <= maxNumThreads; j++)
                {
                    int numThreads;
                    if(j >= size) {
                        numThreads = size;
                    } else {
                        numThreads = j;
                    }
                    omp_set_num_threads(numThreads);

                    times[j-1] += timeCluster(lattice, latticeType, size, chance, &largestClusterSizeThreaded, &percResultThreaded, test, n, numThreads);

                    if(largestClusterSizeSeq != largestClusterSizeThreaded) {
                        printf("\nERROR: CLUSTER SIZE VARIANCE: %llu, %llu\n", largestClusterSizeSeq, largestClusterSizeThreaded);
                    }

                    if(percResultSeq != percResultThreaded) {
                        printf("\nERROR: PERCOLATION VARIANCE: (S) %d, (T) %d\n", percResultSeq, percResultThreaded);
                    }

                    if(percResultThreaded == 1) numPercs[j-1]++;
                    clusterSizes[j-1] += largestClusterSizeThreaded;
                }
            }

            for(int i = 0; i < maxNumThreads; i++) {
                speedup[i] = times[0] / times[i];
            }

            for(int i = 0; i < maxNumThreads; i++) {
                times[i] /= (double) runs;
                clusterSizes[i] /= (double) runs;
            }

            int numPercsSeq = percResultSeq * runs;
            unsigned long long clusterSizeSeq = largestClusterSizeSeq;

            printfCSVLine(maxNumThreads, size, n, allocationTime, times, speedup, numPercsSeq, numPercs, clusterSizeSeq, clusterSizes, fp);

            largestClusterSizeThreaded = 0;

        }
        destroyArray((void**)lattice, size);

        allocationTime = 0;
        largestClusterSizeSeq = 0;
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

    srand(time(NULL));
    srand(0);

    if(rank == 0) {

        FILE *fp = initialiseCSV(options->type, options->probability, options->perlocationType, options->runs, options->maxThreadNum);

        doTests(options->type, options->minSize, options->probability, options->perlocationType, options->runs, options->maxSize, options->minThreadNum, options->maxThreadNum, options->minNodeNum, options->maxNodeNum, rank, fp);

        terminateSlaves(numProcs);

        rewind(fp);
        printFile(fp);
        fclose(fp);

    } else {
        clusterSlave(options->type);
    }


    MPI_Finalize();
}
