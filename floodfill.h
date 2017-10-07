#include <stdio.h>
#include "queue.h"
#include "bond.h"
#include "unionFind.h"

extern unsigned long long floodfillSite(char** array, int size, QUEUE queue);
extern unsigned long long floodfillSiteThread(char** array, int size, QUEUE queue);

extern unsigned long long floodfillBond(BONDSITE** array, int size, QUEUE queue);
// extern unsigned long long floodfillBondThread(BONDSITE** array, int size, QUEUE queue, int leftLim, int rightLim, unsigned long long* setArr,unsigned long long* sizeArr);
void floodfillBondThread(BONDSITE** array, int size, QUEUE queue, int northLim, int southLim, unsigned long long* setArr, unsigned long long* sizeArr);
