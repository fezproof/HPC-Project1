#include "unionFind.h"

//lattice size is just width of lattice
unsigned long long* createSetArr(int numRows, int numCols)
{

    unsigned long long length = (unsigned long long) numRows * (unsigned long long) numCols;

    unsigned long long* setArr = NULL;

    setArr = malloc(length * sizeof(unsigned long long));
    #pragma omp parallel for
        for (unsigned long long i = 0; i < length; i++) {
            setArr[i] = i;
        }

    return setArr;
}

unsigned long long* createSizeArr(int numRows, int numCols)
{
    unsigned long long length = (unsigned long long) numRows * (unsigned long long) numCols;

    unsigned long long* sizeArr;

    sizeArr = malloc(length * sizeof(unsigned long long));
    #pragma omp parallel for
        for (unsigned long long i = 0; i < length; i++) {
            sizeArr[i] = 1;
        }

    return sizeArr;
}


void unionAB(unsigned long long* setArr, unsigned long long* sizeArr,
    unsigned long long numCols, unsigned long long x1, unsigned long long y1,
    unsigned long long x2, unsigned long long y2)
{
    unsigned long long A = x1 * numCols + y1;
    unsigned long long B = x2 * numCols + y2;

    unsigned long long rootA = root(setArr, A);
    unsigned long long rootB = root(setArr, B);

    if(rootA == rootB) return;

    if(sizeArr[rootA] < sizeArr[rootB]) {
        setArr[rootA] = setArr[rootB];
        sizeArr[rootB] += sizeArr[rootA];
    } else {
        setArr[rootB] = setArr[rootA];
        sizeArr[rootA] += sizeArr[rootB];
    }

}

unsigned long long root(unsigned long long* setArr, unsigned long long A)
{

    while(setArr[A] != A)
    {

        setArr[A] = setArr[ setArr[A] ];
        A = setArr[A];

    }
    return A;
}

//lattice size is just width of lattice
int find(unsigned long long* setArr, unsigned long long latticeSize, unsigned long long x1, unsigned long long y1,
    unsigned long long x2, unsigned long long y2)
{
    unsigned long long A = x1 * latticeSize + y1;
    unsigned long long B = x2 * latticeSize + y2;

    if( root(setArr, A) == root(setArr, B) ) {
        return 1;
    } else {
        return 0;
    }
}

unsigned long long findLargestSize(unsigned long long* sizeArr, int latticeSize)
{
    unsigned long long size = (unsigned long long) latticeSize;
    size = size * size;


    // for(unsigned long long i = 0; i < size; i++)
    // {
    //     printf("%llu\t%llu\n", sizeArr[i], i);
    // }

    unsigned long long largestClusterSize = 0;

    #pragma omp parallel for reduction(max: largestClusterSize)
    for(unsigned long long i = 0; i < size; i++) {
        if(sizeArr[i] > largestClusterSize) {
            // printf("i: %llu\n", i);
            largestClusterSize = sizeArr[i];
        }
    }

    return largestClusterSize;
}

void destroySetArr(unsigned long long* setArr)
{
    free(setArr);
}

void destroySizeArr(unsigned long long* sizeArr)
{
    free(sizeArr);
}
