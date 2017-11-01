#include "clusterNode.h"

#define NUM_ROWS_INDEX 0;
#define NUM_COLS_INDEX 1;
#define MSG_SIZE_INDEX 2;
#define NUM_MSGS_INDEX 3;
#define THREADS_INDEX 4;

#define TAG_GENERAL 0;
#define TAG_TERMINATE 1;
#define TAG_ARRAY_COMPLETE 2;

unsigned long long clusterSiteMaster(char** array, int size, int numNodes, int numThreads)
{
    int info[5];
    info[NUM_COLS_INDEX] = size;
    info[THREADS_INDEX] = numThreads;
    int lastMsgSize = 0;

    //determine how many rows to allocate to each processor

    int stdRowsPerProc = 0;
    int lastRowsPerProc = 0;
    int numStdRows = 0;

    if(size % numNodes != 0) {
        if(numNodes <= size/2) {
            stdRowsPerProc = size / numNodes;
            lastRowsPerProc = stdRowsPerProc + (size % numNodes);
            numStdRows = numNodes - 1;
        } else {
            stdRowsPerProc = 2;
            lastRowsPerProc = 1;
            numStdRows = size % numNodes;
        }
    } else {
        stdRowsPerProc = size / numNodes;
        lastRowsPerProc = size / numNodes;
        numStdRows = numNodes;
    }


    //------------------------- Standard procs ----------------------------------


    info[NUM_ROWS_INDEX] = stdRowsPerProc;

    //determine size of messages

    unsigned long long arrSize = (unsigned long long) stdRowsPerProc * (unsigned long long) size;
    if(arrSize > (unsigned long long) INT_MAX) {
        info[MSG_SIZE_INDEX] = INT_MAX;
        info[NUM_MSGS_INDEX] = (int) (arrSize / (unsigned long long) INT_MAX);
        if(arrSize % (unsigned long long) INT_MAX != 0 {
            info[NUM_MSGS_INDEX]++;
            lastMsgSize = (int) (arrSize % (unsigned long long) INT_MAX);
        }
    } else {
        info[MSG_SIZE_INDEX] = (int) arrSize;
        info[NUM_MSGS_INDEX] = 1;
    }

    //send info to procs so they know what to recieve

    for(int i = 1; i < numStdRows; i++) {
        MPI_Send(info, 5, MPI_INT, i, TAG_GENERAL, MPI_COMM_WORLD);
    }

    //send array to procs

    unsigned long long offset = 0;
    for(int i = 1; i < numStdRows; i++) {
        for(int j = 0; j < info[NUM_MSGS_INDEX]-1; j++) {
            MPI_Send(array+offset, info[MSG_SIZE_INDEX], MPI_CHAR, i, TAG_GENERAL, MPI_COMM_WORLD);
            offset += info[MSG_SIZE_INDEX];
        }
        if(lastMsgSize == 0) {
            MPI_Send(array+offset, info[MSG_SIZE_INDEX], MPI_CHAR, i, TAG_GENERAL, MPI_COMM_WORLD);
        } else {
            MPI_Send(array+offset, lastMsgSize, MPI_CHAR, i, TAG_GENERAL, MPI_COMM_WORLD);
        }
        offset = 0;
    }


    //------------------------- Final proc ----------------------------------


    info[NUM_ROWS_INDEX] = lastThreadSize;

    //determine size of messages

    unsigned long long arrSize = (unsigned long long) lastRowsPerProc * (unsigned long long) size;
    if(arrSize > (unsigned long long) INT_MAX) {
        info[MSG_SIZE_INDEX] = INT_MAX;
        info[NUM_MSGS_INDEX] = (int) (arrSize / (unsigned long long) INT_MAX);
        if(arrSize % (unsigned long long) INT_MAX != 0 {
            info[NUM_MSGS_INDEX]++;
            lastMsgSize = (int) (arrSize % (unsigned long long) INT_MAX);
        }
    } else {
        info[MSG_SIZE_INDEX] = (int) arrSize;
        info[NUM_MSGS_INDEX] = 1;
    }

    //send info to procs so they know what to recieve

    for(int i = numStdRows; i < numNodes; i++) {
        MPI_Send(info, 5, MPI_INT, i, TAG_GENERAL, MPI_COMM_WORLD);
    }

    //send array to procs

    offset = 0;
    for(int i = numStdRows; i < numNodes; i++) {
        for(int j = 0; j < info[NUM_MSGS_INDEX]-1; j++) {
            MPI_Send(array+offset, info[MSG_SIZE_INDEX], MPI_CHAR, i, TAG_GENERAL, MPI_COMM_WORLD);
            offset += info[MSG_SIZE_INDEX];
        }
        if(lastMsgSize == 0) {
            MPI_Send(array+offset, info[MSG_SIZE_INDEX], MPI_CHAR, i, TAG_GENERAL, MPI_COMM_WORLD);
        } else {
            MPI_Send(array+offset, lastMsgSize, MPI_CHAR, i, TAG_GENERAL, MPI_COMM_WORLD);
        }
        offset = 0;
    }

    //process the first segment of the array

    for(int i = 1; i < numNodes; i++) {
        //recieve parts of the disjoint set
    }

    //stitch together subarrays with union find

}

unsigned long long clusterSiteSlave()
{
    MPI_Status status;

    int info[5]; //size and numthreads
    int numRows;
    int numCols;
    int msgSize;
    int numMsgs;
    int numThreads;

    char** array;

    while(true) {
        MPI_Recv(info, 5, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if (status.MPI_TAG == TERMINATE) break;

        numRows = info[NUM_ROWS_INDEX];
        numCols = info[NUM_COLS_INDEX];
        msgSize = info[MSG_SIZE_INDEX];
        numMsgs = info[NUM_MSGS_INDEX];
        numThreads = info[THREADS_INDEX];

        //initialise array
        array = createSubLatticeSite(numRows,numCols);

        unsigned long long offset = 0;
        for(int i = 0; i < numMsgs; i++) {
            MPI_Recv(array+offset, msgSize, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            offset += (unsigned long long) msgSize;
        }

        //process array

        //send disjoint set to master

        destroyArraySite(array, arrSize);
    }
}

unsigned long long clusterBondMaster(BONDSITE** array, int size, int numNodes, int numThreads)
{

}

unsigned long long clusterBondSlave()
{

}
