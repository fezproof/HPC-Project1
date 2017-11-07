#include <stdio.h>
#include <time.h>

#include "lattice.h"
#include "percolation.h"
#include "thread.h"
#include "node.h"

extern double timeAllocation(void*** lattice, char latticeType, int size, double chance);
extern double timeCluster(void** lattice, char latticeType, int size, double chance, unsigned long long *largestClusterSize, int* perc, int percType, int numNodes, int numThreads);
