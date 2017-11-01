#include <stdio.h>
#include <time.h>

#include "lattice.h"
#include "percolation.h"
#include "clusterThread.h"
#include "clusterNode.h"

// extern char** createLatticeSite(int size, double chance);

extern double timeAllocateSite(char*** lattice, int size, double chance);
extern double timeSitePerc(char** lattice, int size, int test, int *percResult, int numNodes, int numThreads);
extern double timeSiteCluster(char** lattice, int size, double chance, unsigned long long *largestClusterSize, int numNodes, int numThreads);
