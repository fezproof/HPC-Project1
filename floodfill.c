#include "floodfill.h"

//returns the size of a cluster
//first element in the queue is the start of the cluster
int floodfill(int** array, int size, QUEUE queue)
{
    QUEUE_VERT v;

    int clusterSize = 0;

    int westVert = 0;
    int eastVert = 0;

    while(!queue_isempty(&queue))
    {
        v = dequeue(&queue);
        westVert = v.y;
        eastVert = v.y + 1; //+1 so avoid double-checking


        //move west and check
        while(array[v.x][westVert] == 1)
        {
            array[v.x][westVert] = 2;
            //check south and add to queue
            int southVert = (v.x+1 + size) % size;
            if(array[southVert][westVert] == 1) {
                QUEUE_VERT newV;
                newV.y = westVert;
                newV.x = southVert;
                enqueue(&queue, newV);
            }

            //check north and add to queue
            int northVert = (v.x-1 + size) % size;
            if(array[northVert][westVert] == 1) {
                QUEUE_VERT newV;
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
            int southVert = (v.x+1 + size) % size;
            if(array[southVert][eastVert] == 1) {
                QUEUE_VERT newV;
                newV.y = eastVert;
                newV.x = southVert;
                enqueue(&queue, newV);
            }
            //check north and add to queue
            int northVert = (v.x-1 + size) % size;
            if(array[northVert][eastVert] == 1) {
                QUEUE_VERT newV;
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
int findLargestCluster(int** array, int size)
{
    int largestSize = 0;
    int currentSize = 0;

    QUEUE queue;
    queue_initialise(&queue, size*size);

    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            if(array[i][j] == 1) {

                queue_clear(&queue);
                //vertice from which a cluster is checked
                QUEUE_VERT v;
                v.x = i;
                v.y = j;

                enqueue(&queue, v);

                currentSize = floodfill(array, size, queue);
                if(largestSize < currentSize) {
                    largestSize = currentSize;
                }
            }
        }
    }

    return largestSize;
}
