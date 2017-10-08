#include "cluster.h"

//perform floodfill on each unseen vertice
    //0 - unoccupied
    //1 - occupied
    //2 - occupied and seen
//returns the size of the largest cluster

//returns 0 if failed
unsigned long long findLargestClusterSite(char** array, int size)
{
    char** arrayCpy = NULL;
    arrayCpy = copyLatticeSite(array, size);

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
    destroyArraySite(arrayCpy, size);
    return largestSize;
}


unsigned long long findLargestClusterSiteThread(char** array, int size)
{
    char** arrayCpy = NULL;
    arrayCpy = copyLatticeSiteThread(array, size);

    unsigned long long largestSize = 0;
    unsigned long long currentSize = 0;

    QUEUE queue;

    unsigned long long queueSize = (unsigned long long) size;
    queueSize = queueSize * queueSize;

    if(queue_initialise(&queue, queueSize)) {
        return 0;
    }

    #pragma omp parallel for
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            if(arrayCpy[i][j] == 1) {
                QUEUE_VERT v;
                #pragma omp atomic write
                arrayCpy[i][j] = 2; //mark as seen

                queue_clear(&queue);

                //vertice from which a cluster is checked
                v.x = i;
                v.y = j;

                enqueue(&queue, v);

                currentSize = floodfillSiteThread(arrayCpy, size, queue);
                // printf("\n\tCurrent size: %llu", currentSize);
                // if(currentSize == 0) {
                //     return 0;
                // } else if(largestSize < currentSize) {
                //     largestSize = currentSize;
                // }

                if(largestSize < currentSize) {
                    #pragma omp atomic write
                    largestSize = currentSize;
                }
            }
        }
    }

    queue_free(&queue);
    destroyArraySite(arrayCpy, size);
    return largestSize;
}


unsigned long long findLargestClusterBond(BONDSITE** array, int size)
{
    BONDSITE** arrayCpy = NULL;
    arrayCpy = copyLatticeBond(array, size);

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
    destroyArrayBond(arrayCpy, size);
    return largestSize;
}

unsigned long long findLargestClusterBondThread(BONDSITE** array, int size)
{
    BONDSITE** arrayCpy = NULL;
    arrayCpy = copyLatticeBondThread(array, size);

    unsigned long long largestSize = 0;

    unsigned long long queueSize = (unsigned long long) size;
    queueSize = queueSize * queueSize;

    unsigned long long* setArr = createSetArr(size);
    unsigned long long* sizeArr = createSizeArr(size);

    int numThreads = 0;

    #pragma omp parallel firstprivate(queueSize)
    {
        #pragma omp atomic write
        numThreads = omp_get_num_threads();

        int stdThreadSize = 0;
        int lastThreadSize = 0;
        int numStdThreads = 0;

        if(size % numThreads != 0) {
            if(numThreads <= size/2) {
                stdThreadSize = size / numThreads;
                lastThreadSize = stdThreadSize + (size % numThreads);
                numStdThreads = numThreads - 1;
            } else {
                stdThreadSize = 2;
                lastThreadSize = 1;
                numStdThreads = size % numThreads;
            }
        } else {
            stdThreadSize = size / numThreads;
            lastThreadSize = size / numThreads;
            numStdThreads = numThreads;
        }

        // #pragma omp critical
        // {
        //     printf("Num threads: %d\nstdThreadSize: %d\nlastThreadSize: %d\nnumStdThreads: %d\n", numThreads, stdThreadSize ,lastThreadSize, numStdThreads);
        //
        //     for(int i = 0; i < numStdThreads; i++)
        //     {
        //         printf("%d ",stdThreadSize);
        //     }
        //     for(int i = 0; i < numThreads-numStdThreads; i++)
        //     {
        //         printf("%d ", lastThreadSize);
        //     }
        //     printf("\n\n");
        // }

        #pragma omp for schedule(static, 1)
        for (int n = 0; n < numThreads; n ++) {

            int upBound = 0;
            int downBound = 0;

            QUEUE queue;
            queueSize = queueSize / 1; //TODO reduce the size of the queue to the size of the partition
            queue_initialise(&queue, queueSize);
            // unsigned long long currentSize = 0;
            QUEUE_VERT v;

            if(n < numStdThreads) {
                upBound = n * stdThreadSize;
                downBound = upBound + stdThreadSize - 1;
            } else {
                upBound = (numStdThreads * stdThreadSize) + ((n - numStdThreads) * lastThreadSize);
                downBound = upBound + lastThreadSize - 1;
            }

            #pragma omp critical
            {
                printf("Thread: %d upBound: %d downBound: %d\n", n+1, upBound, downBound);
            }



            // for (int i = n * threadSize; i < (n + 1) * threadSize; i++) {
            //     for(int j = 0; j < size; j++) {

            for (int i = upBound; i <= downBound; i++) {
                for(int j = 0; j < size; j++) {

                    // perror("hey");

                    if(arrayCpy[i][j].seen == 0 && (
                            arrayCpy[i][j].left == 1 ||
                            arrayCpy[i][j].right == 1 ||
                            arrayCpy[i][j].up == 1||
                            arrayCpy[i][j].down == 1)) {

                        arrayCpy[i][j].seen = 1; //mark as seen

                        // perror("hey2");


                        queue_clear(&queue);

                        //vertice from which a cluster is checked
                        v.x = i;
                        v.y = j;

                        enqueue(&queue, v);

                        floodfillBondThread(arrayCpy, size, queue, upBound, downBound, setArr, sizeArr);
                        // printf("\n\tCurrent size: %llu", currentSize);
                        // if(largestSize < currentSize) {
                        //     largestSize = currentSize;
                        // }

                    }
                }
            }
            queue_free(&queue);
        }

    }

    printLatticeBond(arrayCpy, size);

    int threadSize = size / numThreads;
    // int lastThreadSize = size % threadSize;

    // printf("Num threads: %d\nThread size: %d\nLast thread size: %d\n\n", numThreads, threadSize ,lastThreadSize);

    //combine boundaries
    // #pragma omp for
    for(int i = threadSize; i < size - 1 - threadSize; i = i + threadSize) {
        for(int j = 0; j < size; j++) {
            if(arrayCpy[i][j].down == 1) {
                unionAB(setArr, sizeArr, size, i, j, i + 1, j);
            }
        }
    }

    // #pragma omp for
    for(int j = 0; j < size; j++) {
        if(arrayCpy[size-1][j].down == 1) {
            unionAB(setArr, sizeArr, size, size - 1, j, 0, j);
        }
    }

    // if (size <= 64)
    //     printLatticeBond(arrayCpy, size);


    // #pragma omp for schedule(static, threadSize)
    // for(int i = 0; i < size; i++)
    // {
    //     for(int j = 0; j < size; j++)
    //     {
    //         if(arrayCpy[i][j].seen == 0 && (
    //                 arrayCpy[i][j].left == 1 ||
    //                 arrayCpy[i][j].right == 1 ||
    //                 arrayCpy[i][j].up == 1||
    //                 arrayCpy[i][j].down == 1)) {
    //             arrayCpy[i][j].seen = 1; //mark as seen
    //
    //             queue_clear(&queue);
    //
    //             //vertice from which a cluster is checked
    //             v.x = i;
    //             v.y = j;
    //
    //             enqueue(&queue, v);
    //
    //             currentSize = floodfillBondThread(arrayCpy, size, queue);
    //             // printf("\n\tCurrent size: %llu", currentSize);
    //             if(largestSize < currentSize) {
    //                 largestSize = currentSize;
    //             }
    //         }
    //     }
    // }

    largestSize = findLargestCluster(sizeArr, size);


    destroyArrayBond(arrayCpy, size);
    destroySetArr(setArr);
    destroySizeArr(sizeArr);

    return largestSize;
}
