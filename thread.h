#include <omp.h>
#include "floodfill.h"
#include "bond.h"
#include "lattice.h"
#include "unionFind.h"

extern unsigned long long findLargestClusterSite(char** array, int size);
extern unsigned long long findLargestClusterBond(BONDSITE** array, int size);

extern void findLargestClusterSiteThread(char** array, int numRows, int numCols,
    unsigned long long* setArr, unsigned long long* sizeArr, int numThreads);

extern void findLargestClusterBondThread(BONDSITE** array, int numRows, int numCols,
    unsigned long long* setArr, unsigned long long* sizeArr, int numThreads);
