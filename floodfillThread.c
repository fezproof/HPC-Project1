#include "floodfill.h"

//returns the size of a cluster
//first element in the queue is the start of the cluster
unsigned long long floodfillSiteThread(char** array, int size, QUEUE queue)
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
            #pragma omp atomic write
            array[southVert][v.y] = 2;
            newV.y = v.y;
            newV.x = southVert;
            enqueue(&queue, newV);
        }

        //check north of the dequeued site
        if(array[northVert][v.y] == 1) {
            #pragma omp atomic write
            array[northVert][v.y] = 2;
            newV.y = v.y;
            newV.x = northVert;
            enqueue(&queue, newV);
        }

        //move west and check
        while(array[v.x][westVert] == 1)
        {
            #pragma omp atomic write
            array[v.x][westVert] = 2;
            //check south and add to queue
            if(array[southVert][westVert] == 1) {
                #pragma omp atomic write
                array[southVert][westVert] = 2;
                newV.y = westVert;
                newV.x = southVert;
                enqueue(&queue, newV);
            }

            //check north and add to queue
            if(array[northVert][westVert] == 1) {
                #pragma omp atomic write
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
            #pragma omp atomic write
            array[v.x][eastVert] = 2;
            //check south and add to queue
            if(array[southVert][eastVert] == 1) {
                #pragma omp atomic write
                array[southVert][eastVert] = 2;
                newV.y = eastVert;
                newV.x = southVert;
                enqueue(&queue, newV);
            }
            //check north and add to queue
            if(array[northVert][eastVert] == 1) {
                #pragma omp atomic write
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



//returns the size of a cluster
//first element in the queue is the start of the cluster
void floodfillBondThread(BONDSITE** array, int size, QUEUE queue, int northLim, int southLim, unsigned long long* setArr, unsigned long long* sizeArr)
{
    QUEUE_VERT v;

    // unsigned long long clusterSize = 0;

    int curVert = 0;

    int westVert = 0;
    int eastVert = 0;
    int southVert = 0;
    int northVert = 0;

    QUEUE_VERT newV;


    while(!queue_isempty(&queue))
    {
        // clusterSize++;

        v = dequeue(&queue);

        westVert = (v.y-1 + size) % size;
        eastVert = (v.y+1 + size) % size;

        if (v.x + 1 <= southLim) {
            southVert = v.x + 1;
        } else {
            southVert = -1;
        }
        if (v.x - 1 >= northLim) {
            northVert = v.x - 1;
        } else{
            northVert = -1;
        }

        //check south of the dequeued site
        if (southVert != -1)
        if(array[v.x][v.y].down == 1 && array[southVert][v.y].seen == 0) {
            array[southVert][v.y].seen = 1;
            newV.y = v.y;
            newV.x = southVert;
            unionAB(setArr, sizeArr, size, v.x, v.y, southVert, v.y);
            enqueue(&queue, newV);
        }

        //check north of the dequeued site
        if (northVert != -1)
        if(array[v.x][v.y].up == 1 && array[northVert][v.y].seen == 0) {
            array[northVert][v.y].seen = 1;
            newV.y = v.y;
            newV.x = northVert;
            unionAB(setArr, sizeArr, size, v.x, v.y, northVert, v.y);
            enqueue(&queue, newV);
        }

        curVert = v.y;

        //move west and check
        while(array[v.x][curVert].left == 1 && array[v.x][westVert].seen == 0)
        {
            array[v.x][westVert].seen = 1;
            unionAB(setArr, sizeArr, size, v.x, curVert, v.x, westVert);

            //check south and add to queue
            if (southVert != -1)
            if(array[v.x][westVert].down == 1 && array[southVert][westVert].seen == 0) {
                array[southVert][westVert].seen = 1;
                newV.y = westVert;
                newV.x = southVert;
                unionAB(setArr, sizeArr, size, v.x, westVert, southVert, westVert);
                enqueue(&queue, newV);
            }

            //check north and add to queue
            if (northVert != -1)
            if(array[v.x][westVert].up == 1 && array[northVert][westVert].seen == 0) {
                array[northVert][westVert].seen = 1;
                newV.y = westVert;
                newV.x = northVert;
                unionAB(setArr, sizeArr, size, v.x, westVert, northVert, westVert);
                enqueue(&queue, newV);
            }
            curVert = westVert;
            westVert = (westVert-1 + size) % size;
            // clusterSize++;
        }

        curVert = v.y;

        //move east and check
        while(array[v.x][curVert].right == 1 && array[v.x][eastVert].seen == 0)
        {
            array[v.x][eastVert].seen = 1;
            unionAB(setArr, sizeArr, size, v.x, curVert, v.x, eastVert);

            //check south and add to queue
            if (southVert != -1)
            if(array[v.x][eastVert].down == 1 && array[southVert][eastVert].seen == 0) {
                array[southVert][eastVert].seen = 1;
                newV.y = eastVert;
                newV.x = southVert;
                unionAB(setArr, sizeArr, size, v.x, eastVert, southVert, eastVert);
                enqueue(&queue, newV);
            }
            //check north and add to queue
            if (northVert != -1)
            if(array[v.x][eastVert].up == 1 && array[northVert][eastVert].seen == 0) {
                array[northVert][eastVert].seen = 1;
                newV.y = eastVert;
                newV.x = northVert;
                unionAB(setArr, sizeArr, size, v.x, eastVert, northVert, eastVert);
                enqueue(&queue, newV);
            }
            curVert = eastVert;
            eastVert = (eastVert+1 + size) % size;
            // clusterSize++;
        }
    }
    // return clusterSize;
}

























// //returns the size of a cluster
// //first element in the queue is the start of the cluster
// unsigned long long floodfillThread(char** array, int size, QUEUE queue, QUEUE nw, QUEUE ne, QUEUE sw, QUEUE se)
// {
//     QUEUE_VERT v;
//
//     unsigned long long clusterSize = 0;
//
//     int i = 0;
//     int nextWestVert = 0;
//     int nextEastVert = 0;
//
//     int westVert = 0;
//     int eastVert = 0;
//     int southVert = 0;
//     int northVert = 0;
//
//     QUEUE_VERT newV;
//
//     while(!queue_isempty(&queue))
//     {
//         clusterSize++;
//
//         v = dequeue(&queue);
//
//         westVert = v.y;
//         nextWestVert = (v.y - 1 + size) % size;
//         eastVert = v.y;
//         nextEastVert = (v.y + 1 + size) % size;
//
//         southVert = (v.x+1 + size) % size;
//         northVert = (v.x-1 + size) % size;
//
//         //check south of the dequeued site
//         if(array[southVert][v.y] == 1) {
//             array[southVert][v.y] = 2;
//             newV.y = v.y;
//             newV.x = southVert;
//             enqueue(&queue, newV);
//         }
//
//         //check north of the dequeued site
//         if(array[northVert][v.y] == 1) {
//             array[northVert][v.y] = 2;
//             newV.y = v.y;
//             newV.x = northVert;
//             enqueue(&queue, newV);
//         }
//
//         //move from centre to west and check current row
//         while(array[v.x][nextWestVert] == 1)
//         {
//             clusterSize++;
//             westVert = nextWestVert;
//             array[v.x][westVert] = 2;
//             nextWestVert = (westVert-1 + size) % size;
//         }
//
//         //move from centre to east and check current row
//         while(array[v.x][nextEastVert] == 1)
//         {
//             clusterSize++;
//             eastVert = nextEastVert;
//             array[v.x][eastVert] = 2;
//             nextEastVert = (nextEastVert+1 + size) % size;
//         }
//
//         //move from west to centre and check north row
//         i = westVert;
//         if(i <= v.y) {
//             while(i != v.y)
//             {
//                 if(array[northVert][i] == 1) {
//                     array[northVert][i] = 2;
//                     newV.y = i;
//                     newV.x = northVert;
//                     enqueue(&nw, newV);
//                 }
//                 i++;
//             }
//         } else {
//             while(i != size)
//             {
//                 if(array[northVert][i] == 1) {
//                     array[northVert][i] = 2;
//                     newV.y = i;
//                     newV.x = northVert;
//                     enqueue(&nw, newV);
//                 }
//                 i++;
//             }
//             i = 0;
//             while(i != v.y)
//             {
//                 if(array[northVert][i] == 1) {
//                     array[northVert][i] = 2;
//                     newV.y = i;
//                     newV.x = northVert;
//                     enqueue(&nw, newV);
//                 }
//                 i++;
//             }
//         }
//
//         //move from centre to east and check north row
//         i = v.y;
//         if(eastVert >= v.y)
//         {
//             while(i != eastVert)
//             {
//                 i++;
//                 if(array[northVert][i] == 1) {
//                     array[northVert][i] = 2;
//                     newV.y = i;
//                     newV.x = northVert;
//                     enqueue(&ne, newV);
//                 }
//             }
//         } else {
//             while(i != size)
//             {
//                 i++;
//                 if(array[northVert][i] == 1) {
//                     array[northVert][i] = 2;
//                     newV.y = i;
//                     newV.x = northVert;
//                     enqueue(&ne, newV);
//                 }
//             }
//             i = 0;
//             while(i != eastVert)
//             {
//                 i++;
//                 if(array[northVert][i] == 1) {
//                     array[northVert][i] = 2;
//                     newV.y = i;
//                     newV.x = northVert;
//                     enqueue(&ne, newV);
//                 }
//             }
//         }
//
//         //move from west to centre and check south row
//         i = westVert;
//         if(i <= v.y)
//         {
//             while(i != v.y)
//             {
//                 if(array[southVert][i] == 1) {
//                     array[southVert][i] = 2;
//                     newV.y = i;
//                     newV.x = southVert;
//                     enqueue(&sw, newV);
//                 }
//                 i++;
//             }
//         } else {
//             while(i != size)
//             {
//                 if(array[southVert][i] == 1) {
//                     array[southVert][i] = 2;
//                     newV.y = i;
//                     newV.x = southVert;
//                     enqueue(&sw, newV);
//                 }
//                 i++;
//             }
//             i = 0;
//             while(i != v.y)
//             {
//                 if(array[southVert][i] == 1) {
//                     array[southVert][i] = 2;
//                     newV.y = i;
//                     newV.x = southVert;
//                     enqueue(&sw, newV);
//                 }
//                 i++;
//             }
//         }
//
//         i = v.y;
//         if(eastVert >= v.y)
//         {
//             while(i != eastVert)
//             {
//                 i++;
//                 if(array[southVert][i] == 1) {
//                     array[southVert][i] = 2;
//                     newV.y = i;
//                     newV.x = southVert;
//                     enqueue(&se, newV);
//                 }
//             }
//         } else {
//             while(i != size)
//             {
//                 i++;
//                 if(array[southVert][i] == 1) {
//                     array[southVert][i] = 2;
//                     newV.y = i;
//                     newV.x = southVert;
//                     enqueue(&se, newV);
//                 }
//             }
//             i = 0;
//             while(i != eastVert)
//             {
//                 i++;
//                 if(array[southVert][i] == 1) {
//                     array[southVert][i] = 2;
//                     newV.y = i;
//                     newV.x = southVert;
//                     enqueue(&se, newV);
//                 }
//             }
//         }
//
//         while(!queue_isempty(&nw))
//         {
//             enqueue(&queue, dequeue(&nw));
//         }
//         while(!queue_isempty(&ne))
//         {
//             enqueue(&queue, dequeue(&ne));
//         }
//         while(!queue_isempty(&sw))
//         {
//             enqueue(&queue, dequeue(&sw));
//         }
//         while(!queue_isempty(&se))
//         {
//             enqueue(&queue, dequeue(&se));
//         }
//     }
//     return clusterSize;
// }
