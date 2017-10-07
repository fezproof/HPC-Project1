#include <omp.h>
#include "floodfill.h"
#include "bond.h"
#include "lattice.h"
#include "unionFind.h"

//returns the length of the largest cluster
extern unsigned long long findLargestClusterSite(char** array, int size);

//returns the length of the largest cluster
extern unsigned long long findLargestClusterSiteThread(char** array, int size);

extern unsigned long long findLargestClusterBond(BONDSITE** array, int size);

extern unsigned long long findLargestClusterBondThread(BONDSITE** array, int size);
