#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

extern unsigned long long* createSetArr(int numRows, int numCols);
extern unsigned long long* createSizeArr(int numRows, int numCols);


extern void unionAB(unsigned long long* setArr, unsigned long long* sizeArr,
    unsigned long long latticeSize, unsigned long long x1, unsigned long long y1,
    unsigned long long x2, unsigned long long y2);

extern unsigned long long root(unsigned long long* setArr, unsigned long long A);

extern int find(unsigned long long* setArr, unsigned long long latticeSize,
    unsigned long long x1, unsigned long long y1,
    unsigned long long x2, unsigned long long y2);

extern unsigned long long findLargestCluster(unsigned long long* sizeArr, int latticeSize);

extern void destroySetArr(unsigned long long* setArr);
extern void destroySizeArr(unsigned long long* sizeArr);
