#include <stdio.h>
#include "queue.h"
#include "bond.h"

extern unsigned long long floodfillSite(char** array, int size, QUEUE queue);
extern unsigned long long floodfillSiteThread(char** array, int size, QUEUE queue);

extern unsigned long long floodfillBond(BONDSITE** array, int size, QUEUE queue);
extern unsigned long long floodfillBondThread(BONDSITE** array, int size, QUEUE queue);
