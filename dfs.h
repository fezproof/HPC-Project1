#include <omp.h>

#include "stack.h"
#include "bond.h"

extern int dfsUpDownSite(char** array, int size);

extern int dfsLeftRightSite(char** array, int size);

extern int dfsUpDownBond(BONDSITE** array, int size);

extern int dfsLeftRightBond(BONDSITE** array, int size);

extern int dfsUpDownSiteThread(char** array, int size);

extern int dfsLeftRightSiteThread(char** array, int size);

extern int dfsUpDownBondThread(BONDSITE** array, int size);

extern int dfsLeftRightBondThread(BONDSITE** array, int size);
