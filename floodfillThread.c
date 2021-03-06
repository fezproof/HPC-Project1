#include "floodfill.h"

//returns the size of a cluster
//first element in the queue is the start of the cluster
void floodfillSiteThread(char** array, int numRows, int numCols, QUEUE queue, int westLim, int eastLim, unsigned long long* setArr, unsigned long long* sizeArr)
{
    QUEUE_VERT v;

    int westVert = 0;
    int eastVert = 0;
    int southVert = 0;
    int northVert = 0;

    QUEUE_VERT newV;

    int oldVert;

    while(!queue_isempty(&queue))
    {
        v = dequeue(&queue);

        if (v.x < numRows - 1) {
            southVert = v.x + 1;
        } else {
            southVert = -1;
        }
        if (v.x > 0) {
            northVert = v.x - 1;
        } else{
            northVert = -1;
        }

        if (v.y < eastLim) {
            eastVert = v.y + 1;
        } else {
            eastVert = -1;
        }
        if (v.y > westLim) {
            westVert = v.y - 1;
        } else{
            westVert = -1;
        }

        //check south of the dequeued site
        if (southVert != -1)
        if(array[southVert][v.y] == 1) {
            array[southVert][v.y] = 2;
            newV.y = v.y;
            newV.x = southVert;
            unionAB(setArr, sizeArr, numCols, v.x, v.y, southVert, v.y);
            enqueue(&queue, newV);
        }

        //check north of the dequeued site
        if (northVert != -1)
        if(array[northVert][v.y] == 1) {
            array[northVert][v.y] = 2;
            newV.y = v.y;
            newV.x = northVert;
            unionAB(setArr, sizeArr, numCols, v.x, v.y, northVert, v.y);
            enqueue(&queue, newV);
        }

        oldVert = v.y;

        //move west and check
        if(westVert != -1)
        while(array[v.x][westVert] == 1)
        {
            array[v.x][westVert] = 2;
            unionAB(setArr, sizeArr, numCols, v.x, oldVert, v.x, westVert);

            //check south and add to queue
            if (southVert != -1)
            if(array[southVert][westVert] == 1) {
                array[southVert][westVert] = 2;
                newV.y = westVert;
                newV.x = southVert;
                unionAB(setArr, sizeArr, numCols, v.x, westVert, southVert, westVert);
                enqueue(&queue, newV);
            }

            //check north and add to queue
            if (northVert != -1)
            if(array[northVert][westVert] == 1) {
                array[northVert][westVert] = 2;
                newV.y = westVert;
                newV.x = northVert;
                unionAB(setArr, sizeArr, numCols, v.x, westVert, northVert, westVert);
                enqueue(&queue, newV);
            }

            oldVert = westVert;
            westVert--;
            if(westVert < westLim) break;
        }

        oldVert = v.y;

        //move east and check
        if(eastVert != -1)
        while(array[v.x][eastVert] == 1)
        {
            array[v.x][eastVert] = 2;
            unionAB(setArr, sizeArr, numCols, v.x, oldVert, v.x, eastVert);

            //check south and add to queue
            if (southVert != -1)
            if(array[southVert][eastVert] == 1) {
                array[southVert][eastVert] = 2;
                newV.y = eastVert;
                newV.x = southVert;
                unionAB(setArr, sizeArr, numCols, v.x, eastVert, southVert, eastVert);
                enqueue(&queue, newV);
            }
            //check north and add to queue
            if (northVert != -1)
            if(array[northVert][eastVert] == 1) {
                array[northVert][eastVert] = 2;
                newV.y = eastVert;
                newV.x = northVert;
                unionAB(setArr, sizeArr, numCols, v.x, eastVert, northVert, eastVert);
                enqueue(&queue, newV);
            }

            oldVert = eastVert;
            eastVert++;
            if(eastVert > eastLim) break;
        }
    }
}



//returns the size of a cluster
//first element in the queue is the start of the cluster
void floodfillBondThread(BONDSITE** array, int numRows, int numCols, QUEUE queue, int westLim, int eastLim, unsigned long long* setArr, unsigned long long* sizeArr)
{
    QUEUE_VERT v;

    int curVert = 0;

    int westVert = 0;
    int eastVert = 0;
    int southVert = 0;
    int northVert = 0;

    QUEUE_VERT newV;

    while(!queue_isempty(&queue))
    {
        v = dequeue(&queue);

        if (v.x < numRows - 1) {
            southVert = v.x + 1;
        } else {
            southVert = -1;
        }
        if (v.x > 0) {
            northVert = v.x - 1;
        } else{
            northVert = -1;
        }

        if (v.y < eastLim) {
            eastVert = v.y + 1;
        } else {
            eastVert = -1;
        }
        if (v.y > westLim) {
            westVert = v.y - 1;
        } else{
            westVert = -1;
        }

        //check south of the dequeued site
        if (southVert != -1)
        if(array[v.x][v.y].down == 1 && array[southVert][v.y].seen == 0) {
            array[southVert][v.y].seen = 1;
            newV.y = v.y;
            newV.x = southVert;
            unionAB(setArr, sizeArr, numCols, v.x, v.y, southVert, v.y);
            enqueue(&queue, newV);
        }

        // check north of the dequeued site
        if (northVert != -1)
        if(array[v.x][v.y].up == 1 && array[northVert][v.y].seen == 0) {
            array[northVert][v.y].seen = 1;
            newV.y = v.y;
            newV.x = northVert;
            unionAB(setArr, sizeArr, numCols, v.x, v.y, northVert, v.y);
            enqueue(&queue, newV);
        }

        curVert = v.y;

        //move west and check
        if(westVert != -1)
        while(array[v.x][curVert].left == 1 && array[v.x][westVert].seen == 0)
        {
            array[v.x][westVert].seen = 1;
            unionAB(setArr, sizeArr, numCols, v.x, curVert, v.x, westVert);

            //check south and add to queue
            if (southVert != -1)
            if(array[v.x][westVert].down == 1 && array[southVert][westVert].seen == 0) {
                array[southVert][westVert].seen = 1;
                newV.y = westVert;
                newV.x = southVert;
                unionAB(setArr, sizeArr, numCols, v.x, westVert, southVert, westVert);
                enqueue(&queue, newV);
            }

            //check north and add to queue
            if (northVert != -1)
            if(array[v.x][westVert].up == 1 && array[northVert][westVert].seen == 0) {
                array[northVert][westVert].seen = 1;
                newV.y = westVert;
                newV.x = northVert;
                unionAB(setArr, sizeArr, numCols, v.x, westVert, northVert, westVert);
                enqueue(&queue, newV);
            }
            curVert = westVert;
            westVert--;
            if(westVert < westLim) break;
        }

        curVert = v.y;

        //move east and check
        if(eastVert != -1)
        while(array[v.x][curVert].right == 1 && array[v.x][eastVert].seen == 0)
        {
            array[v.x][eastVert].seen = 1;
            unionAB(setArr, sizeArr, numCols, v.x, curVert, v.x, eastVert);

            //check south and add to queue
            if (southVert != -1)
            if(array[v.x][eastVert].down == 1 && array[southVert][eastVert].seen == 0) {
                array[southVert][eastVert].seen = 1;
                newV.y = eastVert;
                newV.x = southVert;
                unionAB(setArr, sizeArr, numCols, v.x, eastVert, southVert, eastVert);
                enqueue(&queue, newV);
            }

            //check north and add to queue
            if (northVert != -1)
            if(array[v.x][eastVert].up == 1 && array[northVert][eastVert].seen == 0) {
                array[northVert][eastVert].seen = 1;
                newV.y = eastVert;
                newV.x = northVert;
                unionAB(setArr, sizeArr, numCols, v.x, eastVert, northVert, eastVert);
                enqueue(&queue, newV);
            }

            curVert = eastVert;
            eastVert++;
            if(eastVert > eastLim) break;
        }
    }
}
