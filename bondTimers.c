#include "bondTimers.h"

double timeAllocateBond(BONDSITE*** lattice, int size, double chance)
{
    struct timespec start, end;
    double allocationTime;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    *lattice = createLatticeBond(size, chance);
    if(*lattice == NULL) {
        printf("Failed whilst creating lattice\n");
    } else {
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);
        allocationTime = (end.tv_sec - start.tv_sec) * 1E3 + (end.tv_nsec - start.tv_nsec) / 1E6;
        // printf("Allocation:\n");
        // printf("\tTime taken: %.6f ms\n", allocationTime);
    }

    return allocationTime;
}

double timePercBond(BONDSITE** lattice, int size, int test, int *percResult)
{
    struct timespec start, end;
    double percolationTime;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    *percResult = percolateBond(lattice, size, test);
    if(*percResult == 2) {
        printf("Failed whilst checking for percolation\n");
    } else {
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);
        percolationTime = (end.tv_sec - start.tv_sec) * 1E3 + (end.tv_nsec - start.tv_nsec) / 1E6;
        // printf("Percolation:\n");
        // printf("\tTime taken: %.6f ms\n", percolationTime);
        // printf("\t%s\n", *percResult ? "SUCCEEDED" : "FAILED");
    }

    return percolationTime;
}

double timePercBondThreaded(BONDSITE** lattice, int size, int test, int *percResult)
{
    struct timespec start, end;
    double percolationTimeThreaded;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    *percResult = percolateBondThread(lattice, size, test);
    if(*percResult == 2) {
        printf("Failed whilst checking for percolation (threaded)\n");
    } else {
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);
        percolationTimeThreaded = (end.tv_sec - start.tv_sec) * 1E3 + (end.tv_nsec - start.tv_nsec) / 1E6;
        // printf("Percolation (Threaded):\n");
        // printf("\tTime taken: %.6f ms\n", percolationTimeThreaded);
        // printf("\t%s\n", *percResult ? "SUCCEEDED" : "FAILED");
    }

    return percolationTimeThreaded;
}

double timeClusterBond(BONDSITE** lattice, int size, double chance, unsigned long long *largestClusterSize)
{
    struct timespec start, end;
    double clusterTime;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    *largestClusterSize = findLargestClusterBond(lattice, size);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    clusterTime = (end.tv_sec - start.tv_sec) * 1E3 + (end.tv_nsec - start.tv_nsec) / 1E6;
    printf("Cluster:\n");
    printf("\tTime taken: %.6f ms\n", clusterTime);
    printf("\tLargest cluster = %lld sites\n", *largestClusterSize);

    return clusterTime;
}

double timeClusterBondThreaded(BONDSITE** lattice, int size, double chance, unsigned long long *largestClusterSize)
{
    struct timespec start, end;
    double clusterTimeThreaded;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    *largestClusterSize = findLargestClusterBondThread(lattice, size);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    clusterTimeThreaded = (end.tv_sec - start.tv_sec) * 1E3 + (end.tv_nsec - start.tv_nsec) / 1E6;
    printf("Cluster (Threaded):\n");
    printf("\tTime taken: %.6f ms\n", clusterTimeThreaded);
    printf("\tLargest cluster = %lld sites\n", *largestClusterSize);

    return clusterTimeThreaded;
}
