#include "clusterNode.h"

#define INFO_SIZE 6

#define NUM_ROWS_INDEX 0
#define NUM_COLS_INDEX 1
#define STD_MSG_SIZE_INDEX 2
#define LAST_MSG_SIZE_INDEX 3
#define NUM_MSGS_INDEX 4
#define THREADS_INDEX 5

#define TAG_GENERAL 0
#define TAG_TERMINATE 1
#define TAG_ARRAY_COMPLETE 2
#define TAG_SET_ARR 3
#define TAG_SIZE_ARR 4

void terminateSlaves(int numSlaves)
{
    // int info[1];
    for(int i = 1; i < numSlaves; i++) {
        MPI_Send(NULL, 0, MPI_INT, i, TAG_TERMINATE, MPI_COMM_WORLD);
    }
}

void determineSizeOfMessages(int numRows, int numCols, int* normMsgSize, int* lastMsgSize, int* numMsgs)
{
    *lastMsgSize = 0;
    unsigned long long arrSize = (unsigned long long) numRows * (unsigned long long) numCols;
    if(arrSize > (unsigned long long) INT_MAX) {
        *normMsgSize = INT_MAX;
        *numMsgs = (int) (arrSize / (unsigned long long) INT_MAX);
        if(arrSize % (unsigned long long) INT_MAX != 0) {
            (*numMsgs)++;
            *lastMsgSize = (int) (arrSize % (unsigned long long) INT_MAX);
        }
    } else {
        *normMsgSize = (int) arrSize;
        *numMsgs = 1;
    }
}

void sendInfoToSlaves(int* info, int firstSlave, int lastSlave)
{
    for(int i = firstSlave; i < lastSlave; i++) {
        MPI_Send(info, INFO_SIZE, MPI_INT, i, TAG_GENERAL, MPI_COMM_WORLD);
    }
}

void sendArrToSlaves(char** array, int firstSlave, int lastSlave, int numMsgs, int stdMsgSize, int lastMsgSize)
{
    unsigned long long offset = 0;
    for(int i = firstSlave; i < lastSlave; i++) {
        printf("Sending arr to slave %d\n", i);
        for(int j = 0; j < numMsgs-1; j++) {
            MPI_Send(array + offset, stdMsgSize, MPI_CHAR, i, TAG_GENERAL, MPI_COMM_WORLD);
            offset += stdMsgSize;
        }
        if(lastMsgSize == 0) {
            MPI_Send(array+offset, stdMsgSize, MPI_CHAR, i, TAG_GENERAL, MPI_COMM_WORLD);
        } else {
            MPI_Send(array+offset, lastMsgSize, MPI_CHAR, i, TAG_GENERAL, MPI_COMM_WORLD);
        }
        offset = 0;
        printf("Sending arr to slave %d - complete\n", i);
    }
}

void portionArray(int numRows, int numSlaves, int* stdRowsPerSlave, int* lastRowsPerSlave, int* numStdSlaves)
{
    if(numRows % numSlaves != 0) {
        if(numSlaves <= numRows/2) {
            *stdRowsPerSlave = numRows / numSlaves;
            *lastRowsPerSlave = *stdRowsPerSlave + (numRows % numSlaves);
            *numStdSlaves = numSlaves - 1;
        } else {
            *stdRowsPerSlave = 2;
            *lastRowsPerSlave = 1;
            *numStdSlaves = numRows % numSlaves;
        }
    } else {
        *stdRowsPerSlave = numRows / numSlaves;
        *lastRowsPerSlave = numRows / numSlaves;
        *numStdSlaves = numSlaves;
    }
}

void recieveSizeAndSetArrs(unsigned long long* setArr, unsigned long long* sizeArr,
unsigned long long* setArrOffset, unsigned long long* sizeArrOffset, int firstSlave, int lastSlave,
int numMsgs, int stdMsgSize, int lastMsgSize)
{
    MPI_Status status;
    unsigned long long offset1 = *setArrOffset;
    unsigned long long offset2 = *sizeArrOffset;

    for(int i = firstSlave; i < lastSlave; i++) {
        //Set arrray
        for(int j = 0; j < numMsgs-1; j++) {
            MPI_Recv(setArr+offset1, stdMsgSize, MPI_UNSIGNED_LONG_LONG, i, TAG_SET_ARR, MPI_COMM_WORLD, &status);
            offset1 += stdMsgSize;
        }
        MPI_Recv(setArr+offset1, lastMsgSize, MPI_UNSIGNED_LONG_LONG, i, TAG_SET_ARR, MPI_COMM_WORLD, &status);
        offset1 += lastMsgSize;

        //Size array
        for(int j = 0; j < numMsgs-1; j++) {
            MPI_Recv(sizeArr+offset2, stdMsgSize, MPI_UNSIGNED_LONG_LONG, i, TAG_SIZE_ARR, MPI_COMM_WORLD, &status);
            offset2 += stdMsgSize;
        }
        MPI_Recv(sizeArr+offset2, lastMsgSize, MPI_UNSIGNED_LONG_LONG, i, TAG_SIZE_ARR, MPI_COMM_WORLD, &status);
        offset2 += lastMsgSize;
    }

    *setArrOffset = offset1;
    *sizeArrOffset = offset2;
}

void sendSizeAndSetArrs(unsigned long long* setArr, unsigned long long* sizeArr,
int numMsgs, int stdMsgSize, int lastMsgSize)
{
    //send setArr to master
    unsigned long long offset = 0;
    for(int i = 0; i < numMsgs-1; i++) {
        MPI_Send(setArr+offset, stdMsgSize, MPI_UNSIGNED_LONG_LONG, 0, TAG_SET_ARR, MPI_COMM_WORLD);
        offset += stdMsgSize;
    }
    MPI_Send(setArr+offset, lastMsgSize, MPI_UNSIGNED_LONG_LONG, 0, TAG_SET_ARR, MPI_COMM_WORLD);
    offset = 0;
    for(int i = 0; i < numMsgs-1; i++) {
        MPI_Send(sizeArr+offset, stdMsgSize, MPI_UNSIGNED_LONG_LONG, 0, TAG_SIZE_ARR, MPI_COMM_WORLD);
        offset += stdMsgSize;
    }
    MPI_Send(sizeArr+offset, lastMsgSize, MPI_UNSIGNED_LONG_LONG, 0, TAG_SIZE_ARR, MPI_COMM_WORLD);
}

unsigned long long clusterSiteMaster(char** array, int size, int numSlaves, int numThreads)
{
    int numCols = size;

    int info1[INFO_SIZE];
    int info2[INFO_SIZE];

    info1[NUM_COLS_INDEX] = numCols;
    info1[THREADS_INDEX] = numThreads;

    info2[NUM_COLS_INDEX] = numCols;
    info2[THREADS_INDEX] = numThreads;

    //------------------ Portion array amongst slaves --------------------------

    int stdRowsPerSlave;
    int lastRowsPerSlave;
    int numStdSlaves;

    portionArray(size, numSlaves, &stdRowsPerSlave, &lastRowsPerSlave, &numStdSlaves);

    //------------------------- Standard procs ---------------------------------

    info1[NUM_ROWS_INDEX] = stdRowsPerSlave;

    determineSizeOfMessages(stdRowsPerSlave, numCols, &info1[STD_MSG_SIZE_INDEX], &info1[LAST_MSG_SIZE_INDEX], &info1[NUM_MSGS_INDEX]);

    sendInfoToSlaves(info1, 1, numStdSlaves);

    sendArrToSlaves(array, 1, numStdSlaves, info1[NUM_MSGS_INDEX], info1[STD_MSG_SIZE_INDEX], info1[LAST_MSG_SIZE_INDEX]);

    //---------------------------- Final procs ----------------------------------


    info2[NUM_ROWS_INDEX] = lastRowsPerSlave;

    determineSizeOfMessages(lastRowsPerSlave, numCols, &info2[STD_MSG_SIZE_INDEX], &info2[LAST_MSG_SIZE_INDEX], &info2[NUM_MSGS_INDEX]);

    sendInfoToSlaves(info2, numStdSlaves, numSlaves);

    sendArrToSlaves(array, numStdSlaves, numSlaves, info2[NUM_MSGS_INDEX], info2[STD_MSG_SIZE_INDEX], info2[LAST_MSG_SIZE_INDEX]);

    printf("got here");

    //--------------- Process the first segment of the array -------------------

    // unsigned long long* setArr = createSetArr(size, size);
    // unsigned long long* sizeArr = createSizeArr(size, size);

    // char** arrCpy = copyLatticeSite(array, stdRowsPerSlave, numCols);

    // findLargestClusterSiteThread(arrCpy, stdRowsPerSlave, numCols, setArr, sizeArr, numThreads);

    // unsigned long long setArrOffset = (unsigned long long) stdRowsPerSlave * (unsigned long long) size;
    // unsigned long long sizeArrOffset = (unsigned long long) stdRowsPerSlave * (unsigned long long) size;

    // recieveSizeAndSetArrs(setArr, sizeArr,
    // &setArrOffset, &sizeArrOffset, 1, numStdSlaves,
    // info1[NUM_MSGS_INDEX], info1[STD_MSG_SIZE_INDEX], info1[LAST_MSG_SIZE_INDEX]);
    //
    // recieveSizeAndSetArrs(setArr, sizeArr,
    // &setArrOffset, &sizeArrOffset, numStdSlaves, numSlaves,
    // info2[NUM_MSGS_INDEX], info2[STD_MSG_SIZE_INDEX], info2[LAST_MSG_SIZE_INDEX]);

    //TODO stitch together subarrays with union find

    // int largestSize = findLargestCluster(sizeArr, size);
    //
    // destroySetArr(setArr);
    // destroySizeArr(sizeArr);

    // return largestSize;

    // int numProcs = 0;
    // MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    // if(numProcs > 1) {
    //     MPI_Recv(info, INFO_SIZE, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    // }



    return 1;
}

void clusterSiteSlave()
{

    MPI_Status status;

    int info[INFO_SIZE]; //size and numthreads
    int numRows;
    int numCols;
    int stdMsgSize;
    // int lastMsgSize;
    int numMsgs;
    // int numThreads;

    char** array;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    while(1) {
        MPI_Recv(info, INFO_SIZE, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if (status.MPI_TAG == TAG_TERMINATE) {
            printf("terminating: %d\n", rank);
            break;
        }

        printf("Slave %d received info: %d, %d, %d, %d, %d, %d\n", rank, info[0], info[1], info[2], info[3], info[4], info[5]);

        numRows = info[NUM_ROWS_INDEX];
        numCols = info[NUM_COLS_INDEX];
        stdMsgSize = info[STD_MSG_SIZE_INDEX];
        // lastMsgSize = info[LAST_MSG_SIZE_INDEX];
        numMsgs = info[NUM_MSGS_INDEX];
        // numThreads = info[THREADS_INDEX];

        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD,&rank);

        //initialise array
        array = createBlankLatticeSite(numRows, numCols);
        printf("--------- Slave (%d) BLANK arr %dx%d--------------\n", rank, numRows, numCols);
        printLatticeSite(array, numRows, numCols);

        unsigned long long offset = 0;
        for(int i = 0; i < numMsgs; i++) {
            MPI_Recv(array + offset, stdMsgSize, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            printf("hoorray\n");
            offset += (unsigned long long) stdMsgSize;
        }

        printf("--------- Slave (%d) arr %dx%d--------------\n", rank, numRows, numCols);
        printf("slave got here 1\n");
        printLatticeSite(array, numRows, numCols);
        printf("slave got here 2\n");




        // unsigned long long* setArr = createSetArr(numRows, numCols);
        // unsigned long long* sizeArr = createSizeArr(numRows, numCols);
        //
        // findLargestClusterSiteThread(array, numRows, numCols, setArr, sizeArr, numThreads);
        //
        // sendSizeAndSetArrs(setArr, sizeArr, numMsgs, stdMsgSize, lastMsgSize);

        // destroyArraySite(array, numRows);
    }
}

unsigned long long clusterBondMaster(BONDSITE** array, int size, int numSlaves, int numThreads)
{
    return 0;
}

void clusterBondSlave()
{
    printf("hey");
}
