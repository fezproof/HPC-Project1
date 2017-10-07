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

    #pragma omp parallel
    {
        int numThreads = omp_get_num_threads();
        int threadSize = size / numThreads;
        #pragma omp for schedule(static, 1)
        for (int n = 0; n < numThreads; n ++) {

            QUEUE queue;
            queue_initialise(&queue, queueSize);
            // unsigned long long currentSize = 0;
            QUEUE_VERT v;

            for (int i = n * threadSize; i < (n + 1) * threadSize; i++) {
                for(int j = 0; j < size; j++) {


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

                        floodfillBondThread(arrayCpy, size, queue, n * threadSize, (n + 1) * threadSize - 1, setArr, sizeArr);
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


    largestSize = findLargestCluster(setArr, size);


    destroyArrayBond(arrayCpy, size);

    return largestSize;
}
