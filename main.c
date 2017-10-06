#include "main.h"

#define RUNS 100
#define MAX_NUM_THREADS 8
#define START_SIZE 64 //64
#define MAX_LATTICE_SIZE 4096/2/2 //131072 16384 8192 4096

FILE* initialiseCSV(char latticeType, double chance, int test, int runs, int maxNumThreads)
{
    char* file = latticeType == 's' ? "site.csv" : "bond.csv";
    FILE *fp = fopen(file,"a+");

    time_t curtime;
    time(&curtime);
    fprintf(fp, "\n\n\n%s", ctime(&curtime));
    fprintf(fp, "\nPercolation,%s", latticeType == 's' ? "Site" : "Bond");
    fprintf(fp, "\nProbability,%f", chance);
    fprintf(fp, "\nPerc type,%d", test);
    fprintf(fp, "\nRuns,%d", runs);

    fprintf(fp,"\nSize,Allocation,,");

    printf("Num threads: %d\n", maxNumThreads);

    for(int i = 1; i <= maxNumThreads; i++) {
        fprintf(fp,"%d: Percolatation,", i);
    }

    fprintf(fp,",");

    for(int i = 1; i <= maxNumThreads; i++) {
        fprintf(fp,"S(%d): Percolation,", i);
    }

    fprintf(fp,",");

    for(int i = 1; i <= maxNumThreads; i++) {
        fprintf(fp,"%d: Cluster,", i);
    }

    fprintf(fp,",");

    for(int i = 1; i <= maxNumThreads; i++) {
        fprintf(fp,"S(%d): Cluster,", i);
    }

    fprintf(fp,",");

    for(int i = 1; i <= maxNumThreads; i++) {
        fprintf(fp,"%d: Total,", i);
    }

    fprintf(fp,",");

    for(int i = 1; i <= maxNumThreads; i++) {
        fprintf(fp,"S(%d): Total,", i);
    }

    fprintf(fp,",,");

    for(int i = 1; i <= maxNumThreads; i++) {
        fprintf(fp,"%d: Times percolated,", i);
    }

    fprintf(fp,",");

    for(int i = 1; i <= maxNumThreads; i++) {
        fprintf(fp,"%d: Avg cluster size,", i);
    }

    fprintf(fp,"\n");

    return fp;
}

void printfCSVLine(FILE *fp, int maxNumThreads,
int size, double allocationTime,
double* percTimes, double* clusterTimes,
double* percSpeedUp, double* clusterSpeedUp,
double* totalTimes, double* totalSpeedUp,
int* timesPerc, unsigned long long* clusterSizes)
{
    fprintf(fp, "%dx%d,%f,,",size,size,allocationTime);

    for(int i = 0; i < maxNumThreads; i++) {
        fprintf(fp,"%f,", percTimes[i]);
    }

    fprintf(fp,",");

    for(int i = 0; i < maxNumThreads; i++) {
        fprintf(fp,"%f,", percSpeedUp[i]);
    }

    fprintf(fp,",");

    for(int i = 0; i < maxNumThreads; i++) {
        fprintf(fp,"%f,", clusterTimes[i]);
    }

    fprintf(fp,",");

    for(int i = 0; i < maxNumThreads; i++) {
        fprintf(fp,"%f,", clusterSpeedUp[i]);
    }

    fprintf(fp,",");

    for(int i = 0; i < maxNumThreads; i++) {
        fprintf(fp,"%f,", totalTimes[i]);
    }

    fprintf(fp,",");

    for(int i = 0; i < maxNumThreads; i++) {
        fprintf(fp,"%f,", totalSpeedUp[i]);
    }

    fprintf(fp,",,");

    for(int i = 0; i < maxNumThreads; i++) {
        fprintf(fp,"%d,", timesPerc[i]);
    }

    fprintf(fp,",");

    for(int i = 0; i < maxNumThreads; i++) {
        fprintf(fp,"%llu,", clusterSizes[i]);
    }

    fprintf(fp,"\n");

}

void sitePerc(int size, double chance, int test, int runs, int maxLatticeSize, int maxNumThreads, FILE *fp)
{
    char** lattice;

    double allocationTime = 0;

    int percResult = 0;
    int percResultThreaded = 0;
    unsigned long long largestClusterSize = 0;
    unsigned long long largestClusterSizeThreaded = 0;

    double percTimes[maxNumThreads-1];
    double clusterTimes[maxNumThreads-1];
    double percSpeedUp[maxNumThreads-1];
    double clusterSpeedUp[maxNumThreads-1];
    double totalTimes[maxNumThreads-1];
    double totalSpeedUp[maxNumThreads-1];
    int timesPerc[maxNumThreads-1];
    unsigned long long clusterSizes[maxNumThreads-1];

    memset(percTimes, 0, sizeof percTimes);
    memset(clusterTimes, 0, sizeof clusterTimes);
    memset(percSpeedUp, 0, sizeof percSpeedUp);
    memset(clusterSpeedUp, 0, sizeof clusterSpeedUp);
    memset(timesPerc, 0, sizeof timesPerc);
    memset(clusterSizes, 0, sizeof clusterSizes);

    do {

        printf("\n------------------------------------------\n");
        printf("Lattice size = %d x %d\n", size, size);

        for(int i = 0; i < runs; i++)
        {
            allocationTime += timeAllocateSite(&lattice, size, chance);
            percTimes[0] += timePercSite(lattice, size, test, &percResult);
            clusterTimes[0] += timeClusterSite(lattice, size, chance, &largestClusterSize);

            if(percResult == 1) timesPerc[0]++;
            clusterSizes[0] += largestClusterSize;

            for(int j = 1; j < maxNumThreads; j++)
            {
                omp_set_num_threads(j+1);
                // printf("Num threads: %d\n", omp_get_num_threads());

                percTimes[j] += timePercSiteThreaded(lattice, size, test, &percResultThreaded);
                clusterTimes[j] += timeClusterSiteThreaded(lattice, size, chance, &largestClusterSizeThreaded);

                if(largestClusterSize != largestClusterSizeThreaded) {
                    printf("\nERROR: CLUSTER SIZE VARIANCE: %llu, %llu\n", largestClusterSize, largestClusterSizeThreaded);
                }

                if(percResult != percResultThreaded) {
                    printf("\nERROR: PERCOLATION VARIANCE");
                }

                if(percResultThreaded == 1) timesPerc[j]++;
                clusterSizes[j] += largestClusterSizeThreaded;
            }
            destroyArraySite(lattice, size);
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

        printfCSVLine(fp, maxNumThreads, size, allocationTime, percTimes, clusterTimes, percSpeedUp, clusterSpeedUp, totalTimes, totalSpeedUp, timesPerc, clusterSizes);

        memset(percTimes, 0, sizeof percTimes);
        memset(clusterTimes, 0, sizeof clusterTimes);
        memset(percSpeedUp, 0, sizeof percSpeedUp);
        memset(clusterSpeedUp, 0, sizeof clusterSpeedUp);
        memset(timesPerc, 0, sizeof timesPerc);
        memset(clusterSizes, 0, sizeof clusterSizes);

        allocationTime = 0;

        largestClusterSize = 0;
        largestClusterSizeThreaded = 0;

        size = size * 2;

    } while (size <= maxLatticeSize);
}

void bondPerc(int size, double chance, int test, int runs, int maxLatticeSize, int maxNumThreads, FILE *fp)
{
    BONDSITE** lattice;

    double allocationTime = 0;
    double percolationTime = 0;
    double percolationTimeThreaded = 0;
    double clusterTime = 0;
    double clusterTimeThreaded = 0;

    int percResult = 0;
    int percResultThreaded = 0;
    int numPercolated = 0;
    int numPercolatedThreaded = 0;
    unsigned long long largestClusterSize = 0;
    unsigned long long largestClusterSizeThreaded = 0;
    unsigned long long sumLargestClusterSize = 0;
    unsigned long long sumLargestClusterSizeThreaded = 0;


    do {

        printf("\n------------------------------------------\n");
        printf("Lattice size = %d x %d\n", size, size);

        for(int i = 0; i < runs; i++)
        {
            allocationTime += timeAllocateBond(&lattice, size, chance);
            percolationTime += timePercBond(lattice, size, test, &percResult);
            percolationTimeThreaded += timePercBondThreaded(lattice, size, test, &percResultThreaded);
            clusterTime += timeClusterBond(lattice, size, chance, &largestClusterSize);
            clusterTimeThreaded += timeClusterBondThreaded(lattice, size, chance, &largestClusterSizeThreaded);

            if(largestClusterSize != largestClusterSizeThreaded) {
                printf("\nERROR: CLUSTER SIZE VARIANCE: %llu, %llu\n", largestClusterSize, largestClusterSizeThreaded);
            }

            sumLargestClusterSize += largestClusterSize;
            sumLargestClusterSizeThreaded += largestClusterSizeThreaded;

            if(percResult == 1) numPercolated++;
            if(percResultThreaded == 1) numPercolatedThreaded++;

            destroyArrayBond(lattice, size);

        }

        allocationTime /= runs;
        percolationTime /= runs;
        percolationTimeThreaded /= runs;
        clusterTime /= runs;
        clusterTimeThreaded /= runs;

        sumLargestClusterSize /= runs;
        sumLargestClusterSizeThreaded /= runs;

        fprintf(fp,"\n%d,%f,%f,%f,%f,%f,%f,%f,,%d,%d,%llu,%llu",
            size,
            allocationTime,
            percolationTime,
            percolationTimeThreaded,
            clusterTime,
            clusterTimeThreaded,
            percolationTime+clusterTime,
            percolationTimeThreaded + clusterTimeThreaded,
            numPercolated,
            numPercolatedThreaded,
            sumLargestClusterSize,
            sumLargestClusterSizeThreaded);

        allocationTime = 0;
        percolationTime = 0;
        percolationTimeThreaded = 0;
        clusterTime = 0;
        clusterTimeThreaded = 0;

        sumLargestClusterSize = 0;
        sumLargestClusterSizeThreaded = 0;

        numPercolated = 0;
        numPercolatedThreaded = 0;

        size = size * 2;

    } while (size <= maxLatticeSize);
}

int main(int argc, char *argv[])
{

    //Prevents the system from changing the number of threads
    omp_set_dynamic(0);

    // int maxNumThreads = 0;
    // if(MAX_NUM_THREADS < omp_get_max_threads()) {
    //     maxNumThreads = MAX_NUM_THREADS;
    // } else {
    //     maxNumThreads = omp_get_max_threads();
    // }

    int maxNumThreads = MAX_NUM_THREADS;

    char latticeType;
    int size;
    double chance;
    int test;
    if (argc != 4) {
        latticeType = 'b';
        chance = 0.5;
        test = 2;
    } else {
        latticeType = argv[1][0];
        chance = atof(argv[2]);
        test = atoi(argv[3]);
    }
    size = START_SIZE;
    srand(time(NULL));

    //Initialise a CSV file
    FILE *fp = initialiseCSV(latticeType, chance, test, RUNS, maxNumThreads);

    if (latticeType == 's') {
        sitePerc(size, chance, test, RUNS, MAX_LATTICE_SIZE, maxNumThreads, fp);
    }
    else {
        bondPerc(size, chance, test, RUNS, MAX_LATTICE_SIZE, maxNumThreads, fp);
    }

    fclose(fp);
}
