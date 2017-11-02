#include "clusterNode.h"

#define INFO_SIZE 7

#define NUM_ROWS_INDEX 0
#define NUM_COLS_INDEX 1
#define STD_MSG_SIZE_INDEX 2
#define LAST_MSG_SIZE_INDEX 3
#define NUM_MSGS_INDEX 4
#define THREADS_INDEX 5
#define MASTER_ROW_POS_INDEX 6

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
        int rowsThatFit = INT_MAX / numCols;
        *normMsgSize = rowsThatFit * numCols;
        *numMsgs = numRows / rowsThatFit;
        if(numRows % rowsThatFit != 0) {
            (*numMsgs)++;
            *lastMsgSize = (numRows - rowsThatFit) * numCols;
        }
    } else {
        *normMsgSize = (int) arrSize;
        *numMsgs = 1;
    }
}

void sendInfoToSlaves(int* info, int firstSlave, int lastSlave, int startRow)
{
    for(int i = firstSlave; i < lastSlave; i++) {
        info[MASTER_ROW_POS_INDEX] = startRow + info[NUM_ROWS_INDEX] * i;
        MPI_Send(info, INFO_SIZE, MPI_INT, i, TAG_GENERAL, MPI_COMM_WORLD);
    }
}

void sendArrToSlaves(char** array, int firstSlave, int lastSlave, int numMsgs, int stdMsgSize, int lastMsgSize, int numCols)
{
    int rowOffset = stdMsgSize / numCols;
    for(int i = firstSlave; i < lastSlave; i++) {

        for(int j = 0; j < numMsgs-1; j++) {
            MPI_Send(&(array[rowOffset][0]), stdMsgSize, MPI_CHAR, i, TAG_GENERAL, MPI_COMM_WORLD);
            rowOffset += stdMsgSize / numCols;
        }
        if(lastMsgSize == 0) {
            MPI_Send(&(array[rowOffset][0]), stdMsgSize, MPI_CHAR, i, TAG_GENERAL, MPI_COMM_WORLD);
            rowOffset += stdMsgSize / numCols;
        } else {
            MPI_Send(&(array[rowOffset][0]), lastMsgSize, MPI_CHAR, i, TAG_GENERAL, MPI_COMM_WORLD);
            rowOffset += lastMsgSize / numCols;
        }
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
    int setOffset = *setArrOffset;
    int sizeOffset = *sizeArrOffset;

    for(int i = firstSlave; i < lastSlave; i++) {
        //Set arrray
        for(int j = 0; j < numMsgs - 1; j++) {
            MPI_Recv(&(setArr[setOffset]), stdMsgSize, MPI_UNSIGNED_LONG_LONG, i, TAG_SET_ARR, MPI_COMM_WORLD, &status);
            setOffset += stdMsgSize;
        }
        if(lastMsgSize == 0) {
            MPI_Recv(&(setArr[setOffset]), stdMsgSize, MPI_UNSIGNED_LONG_LONG, i, TAG_SET_ARR, MPI_COMM_WORLD, &status);
            setOffset += stdMsgSize;
        } else {
            MPI_Recv(&(setArr[setOffset]), lastMsgSize, MPI_UNSIGNED_LONG_LONG, i, TAG_SET_ARR, MPI_COMM_WORLD, &status);
            setOffset += lastMsgSize;
        }


        //Size array
        for(int j = 0; j < numMsgs - 1; j++) {
            MPI_Recv(&(sizeArr[sizeOffset]), stdMsgSize, MPI_UNSIGNED_LONG_LONG, i, TAG_SIZE_ARR, MPI_COMM_WORLD, &status);
            sizeOffset += stdMsgSize;
        }
        if(lastMsgSize == 0) {
            MPI_Recv(&(sizeArr[sizeOffset]), stdMsgSize, MPI_UNSIGNED_LONG_LONG, i, TAG_SET_ARR, MPI_COMM_WORLD, &status);
            sizeOffset += stdMsgSize;
        } else {
            MPI_Recv(&(sizeArr[sizeOffset]), lastMsgSize, MPI_UNSIGNED_LONG_LONG, i, TAG_SET_ARR, MPI_COMM_WORLD, &status);
            sizeOffset += lastMsgSize;
        }


    }


    *setArrOffset = setOffset;
    *sizeArrOffset = sizeOffset;
}

void sendSizeAndSetArrs(unsigned long long* setArr, unsigned long long* sizeArr,
    int numMsgs, int stdMsgSize, int lastMsgSize)
{
    unsigned long long offset = 0;
    for(int i = 0; i < numMsgs - 1; i++) {
        MPI_Send(&(setArr[offset]), stdMsgSize, MPI_UNSIGNED_LONG_LONG, 0, TAG_SET_ARR, MPI_COMM_WORLD);
        offset += stdMsgSize;
    }
    printf("last msg size - set: %d", lastMsgSize);
    if(lastMsgSize == 0) {
        MPI_Send(&(setArr[offset]), stdMsgSize, MPI_UNSIGNED_LONG_LONG, 0, TAG_SET_ARR, MPI_COMM_WORLD);
    } else {
        MPI_Send(&(setArr[offset]), lastMsgSize, MPI_UNSIGNED_LONG_LONG, 0, TAG_SET_ARR, MPI_COMM_WORLD);
    }

    offset = 0;

    for(int i = 0; i < numMsgs - 1; i++) {
        MPI_Send(&(sizeArr[offset]), stdMsgSize, MPI_UNSIGNED_LONG_LONG, 0, TAG_SIZE_ARR, MPI_COMM_WORLD);
        offset += stdMsgSize;
    }
    printf("last msg size - size: %d", lastMsgSize);
    if(lastMsgSize == 0) {
        MPI_Send(&(sizeArr[offset]), stdMsgSize, MPI_UNSIGNED_LONG_LONG, 0, TAG_SET_ARR, MPI_COMM_WORLD);
    } else {
        MPI_Send(&(sizeArr[offset]), lastMsgSize, MPI_UNSIGNED_LONG_LONG, 0, TAG_SET_ARR, MPI_COMM_WORLD);
    }
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

    sendInfoToSlaves(info1, 1, numStdSlaves, 0);

    sendArrToSlaves(array, 1, numStdSlaves, info1[NUM_MSGS_INDEX], info1[STD_MSG_SIZE_INDEX], info1[LAST_MSG_SIZE_INDEX], numCols);

    //---------------------------- Final procs ----------------------------------


    info2[NUM_ROWS_INDEX] = lastRowsPerSlave;

    determineSizeOfMessages(lastRowsPerSlave, numCols, &info2[STD_MSG_SIZE_INDEX], &info2[LAST_MSG_SIZE_INDEX], &info2[NUM_MSGS_INDEX]);

    sendInfoToSlaves(info2, numStdSlaves, numSlaves, numStdSlaves*stdRowsPerSlave);

    sendArrToSlaves(array, numStdSlaves, numSlaves, info2[NUM_MSGS_INDEX], info2[STD_MSG_SIZE_INDEX], info2[LAST_MSG_SIZE_INDEX], numCols);

    printf("--------- Slave (%d) arr %dx%d--------------\n", 0, stdRowsPerSlave, numCols);
    // printLatticeSite(array, stdRowsPerSlave, numCols);
    //--------------- Process the first segment of the array -------------------

    unsigned long long* setArr = createSetArr(size, size);
    unsigned long long* sizeArr = createSizeArr(size, size);

    char** arrCpy = copyLatticeSite(array, stdRowsPerSlave, numCols);

    findLargestClusterSiteThread(arrCpy, stdRowsPerSlave, numCols, setArr, sizeArr, numThreads);

    // for(int i = 0; i < size*size; i++) {
    //     printf("%d\t%llu\t%llu\n", i, setArr[i], sizeArr[i]);
    // }

    unsigned long long setArrOffset = (unsigned long long) stdRowsPerSlave * (unsigned long long) numCols;
    unsigned long long sizeArrOffset = (unsigned long long) stdRowsPerSlave * (unsigned long long) numCols;

    recieveSizeAndSetArrs(setArr, sizeArr,
        &setArrOffset, &sizeArrOffset, 1, numStdSlaves,
        info1[NUM_MSGS_INDEX], info1[STD_MSG_SIZE_INDEX], info1[LAST_MSG_SIZE_INDEX]);

    recieveSizeAndSetArrs(setArr, sizeArr,
        &setArrOffset, &sizeArrOffset, numStdSlaves, numSlaves,
        info2[NUM_MSGS_INDEX], info2[STD_MSG_SIZE_INDEX], info2[LAST_MSG_SIZE_INDEX]);

    for(int i = 0; i < size*size; i++) {
        printf("%d\t%llu\t%llu\n", i, setArr[i], sizeArr[i]);
    }

    //TODO stitch together subarrays with union find

    int largestSize = findLargestCluster(sizeArr, size);

    destroySetArr(setArr);
    destroySizeArr(sizeArr);

    return largestSize;
}

void clusterSiteSlave()
{

    MPI_Status status;

    int info[INFO_SIZE]; //size and numthreads
    int numRows;
    int numCols;
    int stdMsgSize;
    int lastMsgSize;
    int numMsgs;
    int numThreads;
    int masterRowPos;


    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    while(1) {
        MPI_Recv(info, INFO_SIZE, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if (status.MPI_TAG == TAG_TERMINATE) {
            printf("terminating: %d\n", rank);
            break;
        }

        numRows = info[NUM_ROWS_INDEX];
        numCols = info[NUM_COLS_INDEX];
        stdMsgSize = info[STD_MSG_SIZE_INDEX];
        lastMsgSize = info[LAST_MSG_SIZE_INDEX];
        numMsgs = info[NUM_MSGS_INDEX];
        numThreads = info[THREADS_INDEX];
        masterRowPos = info[MASTER_ROW_POS_INDEX];

        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD,&rank);

        //initialise array
        char** array = createBlankLatticeSite(numRows, numCols);

        int rowOffset = 0;
        for(int i = 0; i < numMsgs; i++) {
            MPI_Recv(&array[rowOffset][0], stdMsgSize, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            rowOffset += stdMsgSize / numCols;
        }

        printf("--------- Slave (%d) arr %dx%d--------------\n", rank, numRows, numCols);
        // printLatticeSite(array, numRows, numCols);




        unsigned long long* setArr = createSetArr(numRows, numCols);
        unsigned long long* sizeArr = createSizeArr(numRows, numCols);

        findLargestClusterSiteThread(array, numRows, numCols, setArr, sizeArr, numThreads);

        // for(int i = 0; i < numRows * numCols; i++) {
        //     printf("Slave\t%d\t%llu\t%llu\n", i, setArr[i], sizeArr[i]);
        // }

        //adjust setArr for the root numbers align with the master's setArr
        unsigned long long setSize = (unsigned long long) numRows * (unsigned long long) numCols;
        for(unsigned long long i = 0; i < setSize; i++) {
            setArr[i] += masterRowPos * numCols;
        }

        sendSizeAndSetArrs(setArr, sizeArr, numMsgs, stdMsgSize, lastMsgSize);

        destroySetArr(setArr);
        destroySizeArr(sizeArr);
        destroyArraySite(array, numRows);
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
