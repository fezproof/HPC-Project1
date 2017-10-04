#include <stdio.h>
#include <time.h>

#include "lattice.h"
#include "percolation.h"
#include "cluster.h"

extern BONDSITE** createLatticeBond(int size, double chance);

extern double timeAllocateBond(BONDSITE*** lattice, int size, double chance);
extern double timePercBond(BONDSITE** lattice, int size, int test, int *percResult);
extern double timePercBondThreaded(BONDSITE** lattice, int size, int test, int *percResult);
extern double timeClusterBond(BONDSITE** lattice, int size, double chance, unsigned long long *largestClusterSize);
extern double timeClusterBondThreaded(BONDSITE** lattice, int size, double chance, unsigned long long *largestClusterSize);
