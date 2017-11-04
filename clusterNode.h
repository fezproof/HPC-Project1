#include <omp.h>
#include <mpi.h>
#include <limits.h>
#include "floodfill.h"
#include "bond.h"
#include "lattice.h"
#include "unionFind.h"
#include "clusterThread.h"


extern unsigned long long clusterSiteMaster(char** array, int size, int numNodes, int numThreads, int* perc, int percType);

extern void clusterSiteSlave();

extern unsigned long long clusterBondMaster(BONDSITE** array, int size, int numNodes, int numThreads);

extern void clusterBondSlave();

extern void terminateSlaves(int numNodes);
