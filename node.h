#include <omp.h>
#include <mpi.h>
#include <limits.h>
#include "floodfill.h"
#include "bond.h"
#include "lattice.h"
#include "unionFind.h"
#include "thread.h"

extern unsigned long long clusterMaster(void** array, char latticeType, int size, int numNodes, int numThreads, int* perc, int percType);
extern void clusterSlave(char latticeType);
extern void terminateSlaves(int numNodes);
