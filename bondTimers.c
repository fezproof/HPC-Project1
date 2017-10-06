#include "bondTimers.h"

double timeAllocateBond(BONDSITE*** lattice, int size, double chance)
{
    double startTime;
    double allocationTime;

    startTime = omp_get_wtime();
    *lattice = createLatticeBond(size, chance);
    if(*lattice == NULL) {
        printf("Failed whilst creating lattice\n");
    } else {
        allocationTime = omp_get_wtime() - startTime;
        // printf("Allocation:\n");
        // printf("\tTime taken: %.6f ms\n", allocationTime);
    }

    return allocationTime;
}

double timePercBond(BONDSITE** lattice, int size, int test, int *percResult)
{
    double startTime;
    double percolationTime;

    startTime = omp_get_wtime();
    *percResult = percolateBond(lattice, size, test);
    if(*percResult == 2) {
        printf("Failed whilst checking for percolation\n");
    } else {
        percolationTime = omp_get_wtime() - startTime;
        // printf("Percolation:\n");
        // printf("\tTime taken: %.6f ms\n", percolationTime);
        // printf("\t%s\n", *percResult ? "SUCCEEDED" : "FAILED");
    }

    return percolationTime;
}

double timePercBondThreaded(BONDSITE** lattice, int size, int test, int *percResult)
{
    double startTime;
    double percolationTimeThreaded;

    startTime = omp_get_wtime();
    *percResult = percolateBondThread(lattice, size, test);
    if(*percResult == 2) {
        printf("Failed whilst checking for percolation (threaded)\n");
    } else {
        percolationTimeThreaded = omp_get_wtime() - startTime;
        // printf("Percolation (Threaded):\n");
        // printf("\tTime taken: %.6f ms\n", percolationTimeThreaded);
        // printf("\t%s\n", *percResult ? "SUCCEEDED" : "FAILED");
    }

    return percolationTimeThreaded;
}

double timeClusterBond(BONDSITE** lattice, int size, double chance, unsigned long long *largestClusterSize)
{
    double startTime;
    double clusterTime;

    startTime = omp_get_wtime();
    *largestClusterSize = findLargestClusterBond(lattice, size);
    clusterTime = omp_get_wtime() - startTime;
    // printf("Cluster:\n");
    // printf("\tTime taken: %.6f ms\n", clusterTime);
    // printf("\tLargest cluster = %lld sites\n", *largestClusterSize);

    return clusterTime;
}

double timeClusterBondThreaded(BONDSITE** lattice, int size, double chance, unsigned long long *largestClusterSize)
{
    double startTime;
    double clusterTimeThreaded;

    startTime = omp_get_wtime();
    *largestClusterSize = findLargestClusterBondThread(lattice, size);
    clusterTimeThreaded = omp_get_wtime() - startTime;
    // printf("Cluster (Threaded):\n");
    // printf("\tTime taken: %.6f ms\n", clusterTimeThreaded);
    // printf("\tLargest cluster = %lld sites\n", *largestClusterSize);

    return clusterTimeThreaded;
}
