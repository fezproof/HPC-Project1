#include "main.h"

#define RUNS 30
#define NUMTHREADS 4
#define STARTSIZE 64 //64
#define MAXLATTICESIZE 4096/2 //131072 16384 8192 4096

FILE* initialiseCSV(char latticeType, double chance, int test, int runs)
{
    char* file = latticeType == 's' ? "site.csv" : "bond.csv";
    FILE *fp = fopen(file,"a+");n

    time_t curtime;
    time(&curtime);
    fprintf(fp, "\n\n\n%s", ctime(&curtime));
    fprintf(fp, "\nPercolation,%s", latticeType == 's' ? "Site" : "Bond");
    fprintf(fp, "\nProbability,%f", chance);
    fprintf(fp, "\nPerc type,%d", test);
    fprintf(fp, "\nThreads,%d", NUMTHREADS);
    fprintf(fp, "\nRuns,%d", runs);

    fprintf(fp,"\nSize,Allocation,Percolation,T: Percolation,Cluster,T: Cluster,Total,T: Total,,Times percolated, T: Times percolated,Avg cluster size,T: Avg cluster size");

    return fp;
}

void sitePerc(int size, double chance, int test, int runs, int maxLatticeSize, FILE *fp)
{
    char** lattice;

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
            allocationTime += timeAllocateSite(&lattice, size, chance);
            // percolationTime += timePercSite(lattice, size, test, &percResult);
            // percolationTimeThreaded += timePercSiteThreaded(lattice, size, test, &percResultThreaded);
            clusterTime += timeClusterSite(lattice, size, chance, &largestClusterSize);
            clusterTimeThreaded += timeClusterSiteThreaded(lattice, size, chance, &largestClusterSizeThreaded);

            if(largestClusterSize != largestClusterSizeThreaded) {
                printf("\nERROR: CLUSTER SIZE VARIANCE: %llu, %llu\n", largestClusterSize, largestClusterSizeThreaded);
            }

            sumLargestClusterSize += largestClusterSize;
            sumLargestClusterSizeThreaded += largestClusterSizeThreaded;

            if(percResult == 1) numPercolated++;
            if(percResultThreaded == 1) numPercolatedThreaded++;

            destroyArraySite(lattice, size);

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

        largestClusterSize = 0;
        largestClusterSizeThreaded = 0;

        numPercolated = 0;
        numPercolatedThreaded = 0;

        size = size * 2;

    } while (size < maxLatticeSize);
}

void bondPerc(int size, double chance, int test, int runs, int maxLatticeSize, FILE *fp)
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
    // omp_set_dynamic(NUMTHREADS);
    //Sets the number of threads
    omp_set_num_threads(NUMTHREADS);

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
    size = STARTSIZE;
    srand(time(NULL));

    //Initialise a CSV file
    FILE *fp = initialiseCSV(latticeType, chance, test, RUNS);

    if (latticeType == 's') {
        sitePerc(size, chance, test, RUNS, MAXLATTICESIZE, fp);
    }
    else {
        bondPerc(size, chance, test, RUNS, MAXLATTICESIZE, fp);
    }

    fclose(fp);
}
