#include "thread.h"

//perform floodfill on each unseen vertice
    //0 - unoccupied
    //1 - occupied
    //2 - occupied and seen
//returns the size of the largest cluster

//returns 0 if failed
unsigned long long findLargestClusterSite(char** array, int size)
{
    char** arrayCpy = NULL;
    arrayCpy = copyLatticeSite(array, size, size);

    unsigned long long largestSize = 0;
    unsigned long long currentSize = 0;

    QUEUE queue;

    unsigned long long queueSize = (unsigned long long) size;
    queueSize = queueSize * queueSize;

    if(queue_initialise(&queue, queueSize)) {
        return 0;
    }

    QUEUE_VERT v;

    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            if(arrayCpy[i][j] == 1) {
                arrayCpy[i][j] = 2; //mark as seen

                queue_clear(&queue);

                //vertice from which a cluster is checked
                v.x = i;
                v.y = j;

                enqueue(&queue, v);

                currentSize = floodfillSite(arrayCpy, size, queue);
                // printf("\n\tCurrent size: %llu", currentSize);
                if(currentSize == 0) {
                    return 0;
                } else if(largestSize < currentSize) {
                    largestSize = currentSize;
                }
            }
        }
    }

    queue_free(&queue);
    destroyArray((void**)arrayCpy, size);
    return largestSize;
}

void findLargestClusterSiteThread(char** array, int numRows, int numCols,
    unsigned long long* setArr, unsigned long long* sizeArr, int numThreads)
{

    int stdThreadSize = 0;
    int lastThreadSize = 0;
    int numStdThreads = 0;

    if(numCols % numThreads != 0) {
        if(numThreads <= numCols/2) {
            stdThreadSize = numCols / numThreads;
            lastThreadSize = stdThreadSize + (numCols % numThreads);
            numStdThreads = numThreads - 1;
        } else {
            stdThreadSize = 2;
            lastThreadSize = 1;
            numStdThreads = numCols % numThreads;
        }
    } else {
        stdThreadSize = numCols / numThreads;
        lastThreadSize = numCols / numThreads;
        numStdThreads = numThreads;
    }

    #pragma omp parallel
    {
        #pragma omp for schedule(static, 1)
        for (int n = 0; n < numThreads; n++) {

            int leftBound = 0;
            int rightBound = 0;

            if(n < numStdThreads) {
                leftBound = n * stdThreadSize;
                rightBound = leftBound + stdThreadSize - 1;
            } else {
                leftBound = (numStdThreads * stdThreadSize) + ((n - numStdThreads) * lastThreadSize);
                rightBound = leftBound + lastThreadSize - 1;
            }

            unsigned long long queueSize = (unsigned long long) numRows;
            queueSize = queueSize + queueSize * ((unsigned long long) rightBound - (unsigned long long) leftBound);

            QUEUE queue;
            queue_initialise(&queue, queueSize);
            QUEUE_VERT v;

            for (int i = 0; i < numRows; i++) {
                for(int j = leftBound; j <= rightBound; j++) {
                    if(array[i][j] == 1) {

                        array[i][j] = 2; //mark as seen

                        queue_clear(&queue);

                        //vertice from which a cluster is checked
                        v.x = i;
                        v.y = j;

                        enqueue(&queue, v);

                        floodfillSiteThread(array, numRows, numCols, queue, leftBound, rightBound, setArr, sizeArr);

                    }
                }
            }
            queue_free(&queue);
        }
    }

    int boundLow = 0;

    //combine boundaries
    #pragma omp for schedule(static, 1) private(boundLow)
        for(int i = 0; i < numThreads - 1; i++) {
            if(i < numStdThreads) {
                boundLow = i * stdThreadSize + stdThreadSize - 1;
            } else {
                boundLow = (numStdThreads * stdThreadSize) + ((i - numStdThreads) * lastThreadSize) + lastThreadSize - 1;
            }
            for(int j = 0; j < numRows; j++) {
                if(array[j][boundLow] && array[j][boundLow + 1]) {
                    unionAB(setArr, sizeArr, numCols, j, boundLow, j, boundLow + 1);
                }
            }
        }

}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


void findLargestClusterBondThread(BONDSITE** array, int numRows, int numCols,
    unsigned long long* setArr, unsigned long long* sizeArr, int numThreads)
{
    int stdThreadSize = 0;
    int lastThreadSize = 0;
    int numStdThreads = 0;

    if(numCols % numThreads != 0) {
        if(numThreads <= numCols/2) {
            stdThreadSize = numCols / numThreads;
            lastThreadSize = stdThreadSize + (numCols % numThreads);
            numStdThreads = numThreads - 1;
        } else {
            stdThreadSize = 2;
            lastThreadSize = 1;
            numStdThreads = numCols % numThreads;
        }
    } else {
        stdThreadSize = numCols / numThreads;
        lastThreadSize = numCols / numThreads;
        numStdThreads = numThreads;
    }

    #pragma omp parallel
    {
        #pragma omp for schedule(static, 1)
        for (int n = 0; n < numThreads; n++) {

            int leftBound = 0;
            int rightBound = 0;

            if(n < numStdThreads) {
                leftBound = n * stdThreadSize;
                rightBound = leftBound + stdThreadSize - 1;
            } else {
                leftBound = (numStdThreads * stdThreadSize) + ((n - numStdThreads) * lastThreadSize);
                rightBound = leftBound + lastThreadSize - 1;
            }

            unsigned long long queueSize = (unsigned long long) numRows;
            queueSize = queueSize + queueSize * ((unsigned long long) rightBound - (unsigned long long) leftBound);

            QUEUE queue;
            queue_initialise(&queue, queueSize);
            QUEUE_VERT v;

            for (int i = 0; i < numRows; i++) {
                for(int j = leftBound; j <= rightBound; j++) {

                    if(array[i][j].seen == 0 && (array[i][j].left == 1 || array[i][j].right == 1 ||
                                                    array[i][j].up == 1|| array[i][j].down == 1)) {

                        array[i][j].seen = 1; //mark as seen

                        queue_clear(&queue);

                        //vertice from which a cluster is checked
                        v.x = i;
                        v.y = j;

                        enqueue(&queue, v);

                        floodfillBondThread(array, numRows, numCols, queue, leftBound, rightBound, setArr, sizeArr);
                    }
                }
            }
            queue_free(&queue);
        }

    }

    int bound = 0;
    //combine boundaries
    #pragma omp for schedule(static, 1)
    for(int i = 0; i < numThreads - 1; i++) {
        if(i < numStdThreads) {
            bound = i * stdThreadSize + stdThreadSize - 1;
        } else {
            bound = (numStdThreads * stdThreadSize) + ((i - numStdThreads) * lastThreadSize) + lastThreadSize - 1;
        }
        // printf("bound: %d\t bound2: %d\n", bound, (bound + 1 + size) % size);
        for(int j = 0; j < numRows; j++) {
            if(array[j][bound].right == 1) {
                // printf("hi\n");
                unionAB(setArr, sizeArr, numCols, j, bound, j, bound + 1);
            }
        }
    }
}


unsigned long long findLargestClusterBond(BONDSITE** array, int size)
{
    BONDSITE** arrayCpy = NULL;
    arrayCpy = copyLatticeBond(array, size, size);

    unsigned long long largestSize = 0;
    unsigned long long currentSize = 0;

    QUEUE queue;

    unsigned long long queueSize = (unsigned long long) size;
    queueSize = queueSize * queueSize;


    if(queue_initialise(&queue, queueSize)) {
        return 0;
    }



    QUEUE_VERT v;

    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            if(arrayCpy[i][j].seen == 0 && (
                    arrayCpy[i][j].left == 1 ||
                    arrayCpy[i][j].right == 1 ||
                    arrayCpy[i][j].up == 1||
                    arrayCpy[i][j].down == 1)) {
                arrayCpy[i][j].seen = 1; //mark as seen

                queue_clear(&queue);

                //vertice from which a cluster is checked
                v.x = i;
                v.y = j;

                enqueue(&queue, v);

                currentSize = floodfillBond(arrayCpy, size, queue);
                // printf("\n\tCurrent size: %llu", currentSize);
                if(largestSize < currentSize) {
                    largestSize = currentSize;
                }
            }
        }
    }
    queue_free(&queue);
    destroyArray((void**)arrayCpy, size);
    return largestSize;
}
