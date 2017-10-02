#include "floodfill.h"

//returns the size of a cluster
//first element in the queue is the start of the cluster
unsigned long long floodfill(char** array, int size, QUEUE queue)
{
    QUEUE_VERT v;

    unsigned long long clusterSize = 0;

    int westVert = 0;
    int eastVert = 0;
    int southVert = 0;
    int northVert = 0;

    QUEUE_VERT newV;

    while(!queue_isempty(&queue))
    {
        clusterSize++;

        v = dequeue(&queue);
        westVert = (v.y - 1 + size) % size;
        eastVert = (v.y + 1 + size) % size;

        southVert = (v.x+1 + size) % size;
        northVert = (v.x-1 + size) % size;

        //check south of the dequeued site
        if(array[southVert][v.y] == 1) {
            array[southVert][v.y] = 2;
            newV.y = v.y;
            newV.x = southVert;
            enqueue(&queue, newV);
        }

        //check north of the dequeued site
        if(array[northVert][v.y] == 1) {
            array[northVert][v.y] = 2;
            newV.y = v.y;
            newV.x = northVert;
            enqueue(&queue, newV);
        }


        //move west and check
        while(array[v.x][westVert] == 1)
        {
            array[v.x][westVert] = 2;
            //check south and add to queue
            if(array[southVert][westVert] == 1) {
                array[southVert][westVert] = 2;
                newV.y = westVert;
                newV.x = southVert;
                enqueue(&queue, newV);
            }

            //check north and add to queue
            if(array[northVert][westVert] == 1) {
                array[northVert][westVert] = 2;
                newV.y = westVert;
                newV.x = northVert;
                enqueue(&queue, newV);
            }
            westVert = (westVert-1 + size) % size;
            clusterSize++;
        }

        //move east and check
        while(array[v.x][eastVert] == 1)
        {
            array[v.x][eastVert] = 2;
            //check south and add to queue
            if(array[southVert][eastVert] == 1) {
                array[southVert][eastVert] = 2;
                newV.y = eastVert;
                newV.x = southVert;
                enqueue(&queue, newV);
            }
            //check north and add to queue
            if(array[northVert][eastVert] == 1) {
                array[northVert][eastVert] = 2;
                newV.y = eastVert;
                newV.x = northVert;
                enqueue(&queue, newV);
            }
            eastVert = (eastVert+1 + size) % size;
            clusterSize++;
        }
    }
    return clusterSize;
}


//perform floodfill on each unseen vertice
    //0 - unoccupied
    //1 - occupied
    //2 - occupied and seen
//returns the size of the largest cluster

//returns 0 if failed
unsigned long long findLargestCluster(char** array, int size)
{
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
            if(array[i][j] == 1) {
                array[i][j] = 2; //mark as seen

                queue_clear(&queue);

                //vertice from which a cluster is checked
                v.x = i;
                v.y = j;

                enqueue(&queue, v);

                currentSize = floodfill(array, size, queue);
                if(currentSize == 0) {
                    return 0;
                } else if(largestSize < currentSize) {
                    largestSize = currentSize;
                }
            }
        }
    }

    queue_free(&queue);
    return largestSize;
}
