#include "bondTimers.h"

double timeAllocateBond(BONDSITE*** lattice, int size, double chance)
{
    clock_t start_t, end_t, total_t;

    start_t = clock();
    *lattice = createLatticeBond(size, chance);
    if(*lattice == NULL) {
        printf("Failed whilst creating lattice\n");
    } else {
        end_t = clock();
        total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
        // printf("Allocation:\n");
        // printf("\tTime taken: %.6f ms\n", allocationTime);
    }

    return (double)total_t;
}

double timePercBond(BONDSITE** lattice, int size, int test, int *percResult)
{
    clock_t start_t, end_t, total_t;

    start_t = clock();
    *percResult = percolateBond(lattice, size, test);
    if(*percResult == 2) {
        printf("Failed whilst checking for percolation\n");
    } else {
        end_t = clock();
        total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
        // printf("Percolation:\n");
        // printf("\tTime taken: %.6f ms\n", percolationTime);
        // printf("\t%s\n", *percResult ? "SUCCEEDED" : "FAILED");
    }

    return (double)total_t;
}

double timePercBondThreaded(BONDSITE** lattice, int size, int test, int *percResult)
{
    clock_t start_t, end_t, total_t;

    start_t = clock();
    *percResult = percolateBondThread(lattice, size, test);
    if(*percResult == 2) {
        printf("Failed whilst checking for percolation (threaded)\n");
    } else {
        end_t = clock();
        total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
        // printf("Percolation (Threaded):\n");
        // printf("\tTime taken: %.6f ms\n", percolationTimeThreaded);
        // printf("\t%s\n", *percResult ? "SUCCEEDED" : "FAILED");
    }

    return (double)total_t;
}

double timeClusterBond(BONDSITE** lattice, int size, double chance, unsigned long long *largestClusterSize)
{
    clock_t start_t, end_t, total_t;

    start_t = clock();
    *largestClusterSize = findLargestClusterBond(lattice, size);
    end_t = clock();
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    // printf("Cluster:\n");
    // printf("\tTime taken: %.6f ms\n", clusterTime);
    // printf("\tLargest cluster = %lld sites\n", *largestClusterSize);

    return (double)total_t;
}

double timeClusterBondThreaded(BONDSITE** lattice, int size, double chance, unsigned long long *largestClusterSize)
{
    clock_t start_t, end_t, total_t;

    start_t = clock();
    *largestClusterSize = findLargestClusterBondThread(lattice, size);
    end_t = clock();
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    // printf("Cluster (Threaded):\n");
    // printf("\tTime taken: %.6f ms\n", clusterTimeThreaded);
    // printf("\tLargest cluster = %lld sites\n", *largestClusterSize);

    return (double)total_t;
}
