#include <stdio.h>
#include "queue.h"
#include "bond.h"
#include "unionFind.h"

extern unsigned long long floodfillSite(char** array, int size, QUEUE queue);
extern unsigned long long floodfillBond(BONDSITE** array, int size, QUEUE queue);
extern void floodfillSiteThread(char** array, int numRows, int numCols, QUEUE queue, int northLim, int southLim, unsigned long long* setArr, unsigned long long* sizeArr);
extern void floodfillBondThread(BONDSITE** array, int numRows, int numCols, QUEUE queue, int northLim, int southLim, unsigned long long* setArr, unsigned long long* sizeArr);
