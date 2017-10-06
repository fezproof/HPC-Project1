#include "floodfill.h"

//returns the size of a cluster
//first element in the queue is the start of the cluster
unsigned long long floodfillSite(char** array, int size, QUEUE queue)
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

unsigned long long floodfillBond(BONDSITE** array, int size, QUEUE queue)
{
    QUEUE_VERT v;

    unsigned long long clusterSize = 0;

    int curVert = 0;

    int westVert = 0;
    int eastVert = 0;
    int southVert = 0;
    int northVert = 0;

    QUEUE_VERT newV;

    while(!queue_isempty(&queue))
    {
        clusterSize++;

        v = dequeue(&queue);
        westVert = (v.y-1 + size) % size;
        eastVert = (v.y+1 + size) % size;

        southVert = (v.x+1 + size) % size;
        northVert = (v.x-1 + size) % size;

        //check south of the dequeued site
        if(array[v.x][v.y].down == 1 && array[southVert][v.y].seen == 0) {
            array[southVert][v.y].seen = 1;
            newV.y = v.y;
            newV.x = southVert;
            enqueue(&queue, newV);
        }

        //check north of the dequeued site
        if(array[v.x][v.y].up == 1 && array[northVert][v.y].seen == 0) {
            array[northVert][v.y].seen = 1;
            newV.y = v.y;
            newV.x = northVert;
            enqueue(&queue, newV);
        }

        curVert = v.y;

        //move west and check
        while(array[v.x][curVert].left == 1 && array[v.x][westVert].seen == 0)
        {
            array[v.x][westVert].seen = 1;
            //check south and add to queue
            if(array[v.x][westVert].down == 1 && array[southVert][westVert].seen == 0) {
                array[southVert][westVert].seen = 1;
                newV.y = westVert;
                newV.x = southVert;
                enqueue(&queue, newV);
            }

            //check north and add to queue
            if(array[v.x][westVert].up == 1 && array[northVert][westVert].seen == 0) {
                array[northVert][westVert].seen = 1;
                newV.y = westVert;
                newV.x = northVert;
                enqueue(&queue, newV);
            }
            curVert = westVert;
            westVert = (westVert-1 + size) % size;
            clusterSize++;
        }

        curVert = v.y;

        //move east and check
        while(array[v.x][curVert].right == 1 && array[v.x][eastVert].seen == 0)
        {
            array[v.x][eastVert].seen = 1;
            //check south and add to queue
            if(array[v.x][eastVert].down == 1 && array[southVert][eastVert].seen == 0) {
                array[southVert][eastVert].seen = 1;
                newV.y = eastVert;
                newV.x = southVert;
                enqueue(&queue, newV);
            }
            //check north and add to queue
            if(array[v.x][eastVert].up == 1 && array[northVert][eastVert].seen == 0) {
                array[northVert][eastVert].seen = 1;
                newV.y = eastVert;
                newV.x = northVert;
                enqueue(&queue, newV);
            }
            curVert = eastVert;
            eastVert = (eastVert+1 + size) % size;
            clusterSize++;
        }
    }
    return clusterSize;
}
