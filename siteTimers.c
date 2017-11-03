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

double timeSitePerc(char** lattice, int size, int test, int *percResult, int numNodes, int numThreads)
{
    double startTime;
    double percolationTimeThreaded;

    startTime = omp_get_wtime();
    if(numThreads == 1) {
        *percResult = percolateSite(lattice, size, test);
    } else {
        *percResult = percolateSiteThread(lattice, size, test);
    }
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

double timeSiteCluster(char** lattice, int size, double chance, unsigned long long *largestClusterSize, int numNodes, int numThreads)
{
    double startTime;
    double clusterTimeThreaded = 0;

    startTime = omp_get_wtime();
    if(chance > 0) {

        printf("largestClusterSize: %p\n", (void *) largestClusterSize);
        
        clusterSiteMaster(lattice, size, numNodes, numThreads, largestClusterSize);

        printf("largestClusterSize: %p\n", (void *) largestClusterSize);

        if(*largestClusterSize > 0) {
            clusterTimeThreaded = omp_get_wtime() - startTime;
            // printf("Cluster (Threaded):\n");
            // printf("\tTime taken: %.6f ms\n", clusterTimeThreaded);
            // printf("\tLargest cluster = %lld sites\n", *largestClusterSize);
        } else {
            printf("Failed whilst finding the largest cluster (threaded)\n");
        }
    }
    printf("got here 3.1\n");

    return clusterTimeThreaded;
}
