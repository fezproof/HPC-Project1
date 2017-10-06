#include "siteTimers.h"

double timeAllocateSite(char*** lattice, int size, double chance)
{
    clock_t start_t, end_t, total_t;

    start_t = clock();
    *lattice = createLatticeSite(size, chance);
    if(*lattice == NULL) {
        printf("Failed whilst creating lattice\n");
    } else {
        end_t = clock();
        total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
        // printf("Allocation:\n");
        // printf("\tTime taken: %.6f ms\n", allocationTime);
    }

    return total_t;
}

double timePercSite(char** lattice, int size, int test, int *percResult)
{
    clock_t start_t, end_t, total_t;

    start_t = clock();
    *percResult = percolateSite(lattice, size, test);
    if(*percResult == 2) {
        printf("Failed whilst checking for percolation\n");
    } else {
        end_t = clock();
        total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
        // printf("Percolation:\n");
        // printf("\tTime taken: %.6f ms\n", percolationTime);
        // printf("\t%s\n", *percResult ? "SUCCEEDED" : "FAILED");
    }

    return total_t;
}

double timePercSiteThreaded(char** lattice, int size, int test, int *percResult)
{
    clock_t start_t, end_t, total_t;

    start_t = clock();
    *percResult = percolateSiteThread(lattice, size, test);
    if(*percResult == 2) {
        printf("Failed whilst checking for percolation (threaded)\n");
    } else {
        end_t = clock();
        total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
        // printf("Percolation (Threaded):\n");
        // printf("\tTime taken: %.6f ms\n", percolationTimeThreaded);
        // printf("\t%s\n", *percResult ? "SUCCEEDED" : "FAILED");
    }

    return total_t;
}

double timeClusterSite(char** lattice, int size, double chance, unsigned long long *largestClusterSize)
{
    clock_t start_t, end_t, total_t;

    start_t = clock();
    if(chance > 0) {
        *largestClusterSize = findLargestClusterSite(lattice, size);
        if(*largestClusterSize > 0) {
            end_t = clock();
            total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
            // printf("Cluster:\n");
            // printf("\tTime taken: %.6f ms\n", clusterTime);
            // printf("\tLargest cluster = %lld sites\n", *largestClusterSize);
        } else {
            printf("Failed whilst finding the largest cluster\n");
        }
    } else {
        // printf("Cluster:\n");
        // printf("\tTime taken: 0\n");
        // printf("\tLargest cluster = 0 sites\n");
    }

    return total_t;
}

double timeClusterSiteThreaded(char** lattice, int size, double chance, unsigned long long *largestClusterSize)
{
    clock_t start_t, end_t, total_t;

    start_t = clock();
    if(chance > 0) {
        *largestClusterSize = findLargestClusterSiteThread(lattice, size);
        if(*largestClusterSize > 0) {
            end_t = clock();
            total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
            // printf("Cluster (Threaded):\n");
            // printf("\tTime taken: %.6f ms\n", clusterTimeThreaded);
            // printf("\tLargest cluster = %lld sites\n", *largestClusterSize);
        } else {
            printf("Failed whilst finding the largest cluster (threaded)\n");
        }
    } else {
        // printf("Cluster (Threaded):\n");
        // printf("\tTime taken: 0\n");
        // printf("\tLargest cluster = 0 sites\n");
    }

    return total_t;
}
