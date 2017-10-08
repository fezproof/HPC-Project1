#include "siteTimers.h"

double timeAllocateSite(char*** lattice, int size, double chance)
{
    double startTime;
    double allocationTime;

    startTime = omp_get_wtime();
    *lattice = createLatticeSite(size, chance);
    if(*lattice == NULL) {
        printf("Failed whilst creating lattice\n");
    } else {
        allocationTime = omp_get_wtime() - startTime;
        // printf("Allocation:\n");
        // printf("\tTime taken: %.6f ms\n", allocationTime);
    }

    return allocationTime;
}

double timePercSite(char** lattice, int size, int test, int *percResult)
{
    double startTime;
    double percolationTime;

    startTime = omp_get_wtime();
    *percResult = percolateSite(lattice, size, test);
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

double timePercSiteThreaded(char** lattice, int size, int test, int *percResult)
{
    double startTime;
    double percolationTimeThreaded;

    startTime = omp_get_wtime();
    *percResult = percolateSiteThread(lattice, size, test);
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

double timeClusterSite(char** lattice, int size, double chance, unsigned long long *largestClusterSize)
{
    double startTime;
    double clusterTime;

    startTime = omp_get_wtime();
    if(chance > 0) {
        *largestClusterSize = findLargestClusterSite(lattice, size);
        if(*largestClusterSize > 0) {
            clusterTime = omp_get_wtime() - startTime;
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

    return clusterTime;
}

double timeClusterSiteThreaded(char** lattice, int size, double chance, unsigned long long *largestClusterSize, int numThreads)
{
    double startTime;
    double clusterTimeThreaded;

    startTime = omp_get_wtime();
    if(chance > 0) {
        *largestClusterSize = findLargestClusterSiteThread(lattice, size, numThreads);
        if(*largestClusterSize > 0) {
            clusterTimeThreaded = omp_get_wtime() - startTime;
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

    return clusterTimeThreaded;
}
