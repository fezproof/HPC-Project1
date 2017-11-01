#include <omp.h>
#include <mpi.h>
#include "floodfill.h"
#include "bond.h"
#include "lattice.h"
#include "unionFind.h"


extern unsigned long long clusterSiteMaster(char** array, int size, int numNodes, int numThreads);

extern unsigned long long clusterSiteSlave();

extern unsigned long long clusterBondMaster(BONDSITE** array, int size, int numNodes, int numThreads);

extern unsigned long long clusterBondSlave();
