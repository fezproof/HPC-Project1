#include <omp.h>
#include <mpi.h>
#include <limits.h>
#include "floodfill.h"
#include "bond.h"
#include "lattice.h"
#include "unionFind.h"
#include "clusterThread.h"


extern void clusterSiteMaster(char** array, int size, int numNodes, int numThreads, unsigned long long* largestClusterSize);

extern void clusterSiteSlave();

extern unsigned long long clusterBondMaster(BONDSITE** array, int size, int numNodes, int numThreads);

extern void clusterBondSlave();

extern void terminateSlaves(int numNodes);
