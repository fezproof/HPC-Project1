#include "timers.h"

double timeAllocation(void*** lattice, char latticeType, int size, double chance)
{
    double startTime;
    double allocationTime;

    startTime = omp_get_wtime();
    if(latticeType == 's') {
        *lattice = (void**)createLatticeSite(size, chance);
    } else {
        *lattice = (void**)createLatticeBond(size, chance);
    }
    if(*lattice == NULL) {
        printf("Failed whilst creating lattice\n");
    } else {
        allocationTime = omp_get_wtime() - startTime;
        // printf("Allocation:\n");
        // printf("\tTime taken: %.6f ms\n", allocationTime);
    }

    return allocationTime;
}

double timeCluster(void** lattice, char latticeType, int size, double chance, unsigned long long *largestClusterSize, int* perc, int percType, int numNodes, int numThreads)
{
    double startTime;
    double clusterTimeThreaded = 0;

    startTime = omp_get_wtime();
    if(chance > 0) {

        *largestClusterSize = clusterMaster(lattice, latticeType, size, numNodes, numThreads, perc, percType);

        if(*largestClusterSize > 0) {
            clusterTimeThreaded = omp_get_wtime() - startTime;
            // printf("Cluster (Threaded):\n");
            // printf("\tTime taken: %.6f ms\n", clusterTimeThreaded);
            // printf("\tLargest cluster = %lld sites\n", *largestClusterSize);
        } else {
            printf("Failed whilst finding the largest cluster (threaded)\n");
        }
    }

    return clusterTimeThreaded;
}
