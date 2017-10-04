#include "floodfill.h"

//returns the size of a cluster
//first element in the queue is the start of the cluster
unsigned long long floodfill(char** array, int size, QUEUE queue)
{
    QUEUE_VERT v;

    unsigned long long clusterSize = 0;

    int i = 0;
    int nextWestVert = 0;
    int nextEastVert = 0;

    int westVert = 0;
    int eastVert = 0;
    int southVert = 0;
    int northVert = 0;

    QUEUE_VERT newV;

    while(!queue_isempty(&queue))
    {
        clusterSize++;

        v = dequeue(&queue);

        westVert = v.y;
        nextWestVert = (v.y - 1 + size) % size;
        eastVert = v.y;
        nextEastVert = (v.y + 1 + size) % size;

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

        //move from centre to west and check current row
        while(array[v.x][nextWestVert] == 1)
        {
            clusterSize++;
            westVert = nextWestVert;
            array[v.x][westVert] = 2;
            nextWestVert = (westVert-1 + size) % size;
        }

        //move from centre to east and check current row
        while(array[v.x][nextEastVert] == 1)
        {
            clusterSize++;
            eastVert = nextEastVert;
            array[v.x][eastVert] = 2;
            nextEastVert = (nextEastVert+1 + size) % size;
        }

        //move from west to centre and check north row
        i = westVert;
        if(i <= v.y) {
            while(i != v.y)
            {
                if(array[northVert][i] == 1) {
                    array[northVert][i] = 2;
                    newV.y = i;
                    newV.x = northVert;
                    enqueue(&queue, newV);
                }
                i++;
            }
        } else {
            while(i != size)
            {
                if(array[northVert][i] == 1) {
                    array[northVert][i] = 2;
                    newV.y = i;
                    newV.x = northVert;
                    enqueue(&queue, newV);
                }
                i++;
            }
            i = 0;
            while(i != v.y)
            {
                if(array[northVert][i] == 1) {
                    array[northVert][i] = 2;
                    newV.y = i;
                    newV.x = northVert;
                    enqueue(&queue, newV);
                }
                i++;
            }
        }

        //move from centre to east and check north row
        i = v.y;
        if(eastVert >= v.y)
        {
            while(i != eastVert)
            {
                i++;
                if(array[northVert][i] == 1) {
                    array[northVert][i] = 2;
                    newV.y = i;
                    newV.x = northVert;
                    enqueue(&queue, newV);
                }
            }
        } else {
            while(i != size)
            {
                i++;
                if(array[northVert][i] == 1) {
                    array[northVert][i] = 2;
                    newV.y = i;
                    newV.x = northVert;
                    enqueue(&queue, newV);
                }
            }
            i = 0;
            while(i != eastVert)
            {
                i++;
                if(array[northVert][i] == 1) {
                    array[northVert][i] = 2;
                    newV.y = i;
                    newV.x = northVert;
                    enqueue(&queue, newV);
                }
            }
        }

        //move from west to centre and check south row
        i = westVert;
        if(i <= v.y)
        {
            while(i != v.y)
            {
                if(array[southVert][i] == 1) {
                    array[southVert][i] = 2;
                    newV.y = i;
                    newV.x = southVert;
                    enqueue(&queue, newV);
                }
                i++;
            }
        } else {
            while(i != size)
            {
                if(array[southVert][i] == 1) {
                    array[southVert][i] = 2;
                    newV.y = i;
                    newV.x = southVert;
                    enqueue(&queue, newV);
                }
                i++;
            }
            i = 0;
            while(i != v.y)
            {
                if(array[southVert][i] == 1) {
                    array[southVert][i] = 2;
                    newV.y = i;
                    newV.x = southVert;
                    enqueue(&queue, newV);
                }
                i++;
            }
        }

        i = v.y;
        if(eastVert >= v.y)
        {
            while(i != eastVert)
            {
                i++;
                if(array[southVert][i] == 1) {
                    array[southVert][i] = 2;
                    newV.y = i;
                    newV.x = southVert;
                    enqueue(&queue, newV);
                }
            }
        } else {
            while(i != size)
            {
                i++;
                if(array[southVert][i] == 1) {
                    array[southVert][i] = 2;
                    newV.y = i;
                    newV.x = southVert;
                    enqueue(&queue, newV);
                }
            }
            i = 0;
            while(i != eastVert)
            {
                i++;
                if(array[southVert][i] == 1) {
                    array[southVert][i] = 2;
                    newV.y = i;
                    newV.x = southVert;
                    enqueue(&queue, newV);
                }
            }
        }
    }
    return clusterSize;
}
