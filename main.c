#include "main.h"

#define RUNS 1
#define MAX_NUM_THREADS 16

#define START_SIZE 16 //64
#define MAX_LATTICE_SIZE 2  //131072 16384 8192 4096

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

    fprintf(fp,"\nSize,Allocation,,");

    printf("Num threads: %d\n", maxNumThreads);

    for(int i = 1; i <= maxNumThreads; i++) {
        fprintf(fp,"%d: Percolation,", i);
    }

    fprintf(fp,",0,");

    for(int i = 1; i <= maxNumThreads; i++) {
        fprintf(fp,"%d,", i);
    }

    fprintf(fp,",");

    for(int i = 1; i <= maxNumThreads; i++) {
        fprintf(fp,"%d: Cluster,", i);
    }

    fprintf(fp,",0,");

    for(int i = 1; i <= maxNumThreads; i++) {
        fprintf(fp,"%d,", i);
    }

    fprintf(fp,",");

    for(int i = 1; i <= maxNumThreads; i++) {
        fprintf(fp,"%d: Total,", i);
    }

    fprintf(fp,",0,");

    for(int i = 1; i <= maxNumThreads; i++) {
        fprintf(fp,"%d,", i);
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

    do {

        memset(percTimes, 0, sizeof percTimes);
        memset(clusterTimes, 0, sizeof clusterTimes);
        memset(percSpeedUp, 0, sizeof percSpeedUp);
        memset(clusterSpeedUp, 0, sizeof clusterSpeedUp);
        memset(timesPerc, 0, sizeof timesPerc);
        memset(clusterSizes, 0, sizeof clusterSizes);

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
                if(j+1 > size) {
                    omp_set_num_threads(size);
                } else {
                    omp_set_num_threads(j+1);
                }
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

    do {

        memset(percTimes, 0, sizeof percTimes);
        memset(clusterTimes, 0, sizeof clusterTimes);
        memset(percSpeedUp, 0, sizeof percSpeedUp);
        memset(clusterSpeedUp, 0, sizeof clusterSpeedUp);
        memset(timesPerc, 0, sizeof timesPerc);
        memset(clusterSizes, 0, sizeof clusterSizes);

        printf("\n------------------------------------------\n");
        printf("Lattice size = %d x %d\n", size, size);

        for(int i = 0; i < runs; i++)
        {
            allocationTime += timeAllocateBond(&lattice, size, chance);
            percTimes[0] += timePercBond(lattice, size, test, &percResult);
            clusterTimes[0] += timeClusterBond(lattice, size, chance, &largestClusterSize);

            if(percResult == 1) timesPerc[0]++;
            clusterSizes[0] += largestClusterSize;

            for(int j = 1; j < maxNumThreads; j++)
            {
                if(j+1 > size) {
                    omp_set_num_threads(size);
                } else {
                    omp_set_num_threads(j+1);
                }

                percTimes[j] += timePercBondThreaded(lattice, size, test, &percResultThreaded);
                clusterTimes[j] += timeClusterBondThreaded(lattice, size, chance, &largestClusterSizeThreaded);

                if(largestClusterSize != largestClusterSizeThreaded) {
                    // printf("\nERROR: CLUSTER SIZE VARIANCE: %llu, %llu\n", largestClusterSize, largestClusterSizeThreaded);
                }

                if(percResult != percResultThreaded) {
                    printf("\nERROR: PERCOLATION VARIANCE");
                }

                if(percResultThreaded == 1) timesPerc[j]++;
                clusterSizes[j] += largestClusterSizeThreaded;

                printf("\n------------------------------------------\n");
                
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

        printfCSVLine(fp, maxNumThreads, size, allocationTime, percTimes, clusterTimes, percSpeedUp, clusterSpeedUp, totalTimes, totalSpeedUp, timesPerc, clusterSizes);

        allocationTime = 0;

        largestClusterSize = 0;
        largestClusterSizeThreaded = 0;

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

    printf("Size of int: %ld", sizeof(int));
    printf("Size of luu: %ld", sizeof(unsigned long long));

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
