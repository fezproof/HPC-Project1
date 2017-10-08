#include <stdio.h>
#include <time.h>

#include "lattice.h"
#include "percolation.h"
#include "cluster.h"

extern char** createLatticeSite(int size, double chance);

extern double timeAllocateSite(char*** lattice, int size, double chance);
extern double timePercSite(char** lattice, int size, int test, int *percResult);
extern double timePercSiteThreaded(char** lattice, int size, int test, int *percResult);
extern double timeClusterSite(char** lattice, int size, double chance, unsigned long long *largestClusterSize);
extern double timeClusterSiteThreaded(char** lattice, int size, double chance, unsigned long long *largestClusterSize, int numThreads);
