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
#define TAG_INFO 5
#define TAG_ARR 6

void terminateSlaves(int numSlaves)
{
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
        info[MASTER_ROW_POS_INDEX] = startRow + info[NUM_ROWS_INDEX] * (i - firstSlave);
        // printf("Master row pos for slave %d: %d\n", i, info[MASTER_ROW_POS_INDEX]);
        MPI_Send(info, INFO_SIZE, MPI_INT, i, TAG_INFO, MPI_COMM_WORLD);
    }
}

void sendArrToSlaves(char** array, int* rowOffset, int firstSlave, int lastSlave, int numMsgs, int stdMsgSize, int lastMsgSize, int numCols, MPI_Request* req)
{
    for(int i = firstSlave; i < lastSlave; i++) {

        for(int j = 0; j < numMsgs-1; j++) {
            MPI_Isend(&(array[*rowOffset][0]), stdMsgSize, MPI_CHAR, i, TAG_ARR, MPI_COMM_WORLD, req);
            (*rowOffset) += stdMsgSize / numCols;
        }
        if(lastMsgSize == 0) {
            MPI_Isend(&(array[*rowOffset][0]), stdMsgSize, MPI_CHAR, i, TAG_ARR, MPI_COMM_WORLD, req);
            (*rowOffset) += stdMsgSize / numCols;
        } else {
            MPI_Isend(&(array[*rowOffset][0]), lastMsgSize, MPI_CHAR, i, TAG_ARR, MPI_COMM_WORLD, req);
            (*rowOffset) += lastMsgSize / numCols;
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

void recieveArray1dUll(unsigned long long* arr, unsigned long long* offset,
    int firstSlave, int lastSlave, int numMsgs, int stdMsgSize, int lastMsgSize,
    int tag, MPI_Request* req, int* curMsgNum)
{
    for(int i = firstSlave; i < lastSlave; i++) {
        for(int j = 0; j < numMsgs - 1; j++) {
            MPI_Irecv(&(arr[*offset]), stdMsgSize, MPI_UNSIGNED_LONG_LONG, i, tag, MPI_COMM_WORLD, &req[*curMsgNum]);
            (*offset) += stdMsgSize;
            (*curMsgNum)++;
        }
        if(lastMsgSize == 0) {
            MPI_Irecv(&(arr[*offset]), stdMsgSize, MPI_UNSIGNED_LONG_LONG, i, tag, MPI_COMM_WORLD, &req[*curMsgNum]);
            (*offset) += stdMsgSize;
            (*curMsgNum)++;
        } else {
            MPI_Irecv(&(arr[*offset]), lastMsgSize, MPI_UNSIGNED_LONG_LONG, i, tag, MPI_COMM_WORLD, &req[*curMsgNum]);
            (*offset) += lastMsgSize;
            (*curMsgNum)++;
        }
    }
}

void sendArray1dUll(unsigned long long* arr, int numMsgs, int stdMsgSize, int lastMsgSize, int tag)
{
    unsigned long long offset = 0;
    for(int i = 0; i < numMsgs - 1; i++) {
        MPI_Send(&(arr[offset]), stdMsgSize, MPI_UNSIGNED_LONG_LONG, 0, tag, MPI_COMM_WORLD);
        offset += stdMsgSize;
    }
    if(lastMsgSize == 0) {
        MPI_Send(&(arr[offset]), stdMsgSize, MPI_UNSIGNED_LONG_LONG, 0, tag, MPI_COMM_WORLD);
    } else {
        MPI_Send(&(arr[offset]), lastMsgSize, MPI_UNSIGNED_LONG_LONG, 0, tag, MPI_COMM_WORLD);
    }
}

char** receiveArrayPortion(int numMsgs, int stdMsgSize, int numRows, int numCols)
{
    char** array = createBlankLatticeSite(numRows, numCols);
    MPI_Status status;
    int rowOffset = 0;

    for(int i = 0; i < numMsgs; i++) {
        MPI_Recv(&array[rowOffset][0], stdMsgSize, MPI_CHAR, 0, TAG_ARR, MPI_COMM_WORLD, &status);
        rowOffset += stdMsgSize / numCols;
    }

    return array;
}

void adjustSetArr(unsigned long long* setArr, int numRows, int numCols, int masterRowPos)
{
    unsigned long long setSize = (unsigned long long) numRows * (unsigned long long) numCols;
    for(unsigned long long i = 0; i < setSize; i++) {
        setArr[i] += masterRowPos * numCols;
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

    info1[NUM_ROWS_INDEX] = stdRowsPerSlave;
    info2[NUM_ROWS_INDEX] = lastRowsPerSlave;

    //------------------------- Standard procs ---------------------------------

    MPI_Request sendReq;
    int rowOffet = stdRowsPerSlave;

    determineSizeOfMessages(stdRowsPerSlave, numCols, &info1[STD_MSG_SIZE_INDEX], &info1[LAST_MSG_SIZE_INDEX], &info1[NUM_MSGS_INDEX]);
    sendInfoToSlaves(info1, 1, numStdSlaves, stdRowsPerSlave);
    sendArrToSlaves(array, &rowOffet, 1, numStdSlaves, info1[NUM_MSGS_INDEX], info1[STD_MSG_SIZE_INDEX], info1[LAST_MSG_SIZE_INDEX], numCols, &sendReq);

    //---------------------------- Final procs ----------------------------------

    determineSizeOfMessages(lastRowsPerSlave, numCols, &info2[STD_MSG_SIZE_INDEX], &info2[LAST_MSG_SIZE_INDEX], &info2[NUM_MSGS_INDEX]);
    sendInfoToSlaves(info2, numStdSlaves, numSlaves, numStdSlaves*stdRowsPerSlave);
    sendArrToSlaves(array, &rowOffet, numStdSlaves, numSlaves, info2[NUM_MSGS_INDEX], info2[STD_MSG_SIZE_INDEX], info2[LAST_MSG_SIZE_INDEX], numCols, &sendReq);

    //--------------- Process the first segment of the array -------------------

    printf("--------- Master arr %dx%d--------------\n", stdRowsPerSlave, numCols);
    // printLatticeSite(array, stdRowsPerSlave, numCols);

    unsigned long long* setArr = createSetArr(size, size);
    unsigned long long* sizeArr = createSizeArr(size, size);

    char** arrCpy = copyLatticeSite(array, stdRowsPerSlave, numCols);

    findLargestClusterSiteThread(arrCpy, stdRowsPerSlave, numCols, setArr, sizeArr, numThreads);

    int numMsgsToReceive = 2 * ( (info1[NUM_MSGS_INDEX] * (numStdSlaves-1)) + (info2[NUM_MSGS_INDEX] * (numSlaves-numStdSlaves)) );
    int curMsgNum = 0;

    MPI_Status* status = malloc(numMsgsToReceive * sizeof(MPI_Status));
    MPI_Request* receiveReq = malloc(numMsgsToReceive * sizeof(MPI_Request));

    unsigned long long setArrOffset = (unsigned long long) stdRowsPerSlave * (unsigned long long) numCols;
    unsigned long long sizeArrOffset = (unsigned long long) stdRowsPerSlave * (unsigned long long) numCols;

    recieveArray1dUll(setArr, &setArrOffset, 1, numStdSlaves, info1[NUM_MSGS_INDEX], info1[STD_MSG_SIZE_INDEX], info1[LAST_MSG_SIZE_INDEX], TAG_SET_ARR, receiveReq, &curMsgNum);
    recieveArray1dUll(sizeArr, &sizeArrOffset, 1, numStdSlaves, info1[NUM_MSGS_INDEX], info1[STD_MSG_SIZE_INDEX], info1[LAST_MSG_SIZE_INDEX], TAG_SIZE_ARR, receiveReq, &curMsgNum);
    recieveArray1dUll(setArr, &setArrOffset, numStdSlaves, numSlaves, info2[NUM_MSGS_INDEX], info2[STD_MSG_SIZE_INDEX], info2[LAST_MSG_SIZE_INDEX], TAG_SET_ARR, receiveReq, &curMsgNum);
    recieveArray1dUll(sizeArr, &sizeArrOffset, numStdSlaves, numSlaves, info2[NUM_MSGS_INDEX], info2[STD_MSG_SIZE_INDEX], info2[LAST_MSG_SIZE_INDEX], TAG_SIZE_ARR, receiveReq, &curMsgNum);

    printf("Calculated num messages: %d\n", numMsgsToReceive);
    printf("Actual num messages: %d\n", curMsgNum);

    MPI_Waitall(numMsgsToReceive, receiveReq, status);

    // for(int i = 0; i < size*size; i++) {
    //     printf("%d\t%llu\t%llu\n", i, setArr[i], sizeArr[i]);
    // }

    // unsigned long long start = (unsigned long long) size;
    // start = start * start;
    // unsigned long long max = start;
    // start -= 10;
    // for(unsigned long long i = start; i < max; i++) {
    //     printf("%llu\t%llu\t%llu\n", i, setArr[i], sizeArr[i]);
    // }

    printf("got here\n");

    int boundLow = 0;

    //combine boundaries
    #pragma omp for schedule(static, 1) private(boundLow)
        for(int i = 0; i < numSlaves - 1; i++) {
            if(i < numStdSlaves) {
                boundLow = i * stdRowsPerSlave + stdRowsPerSlave - 1;
            } else {
                boundLow = (numStdSlaves * stdRowsPerSlave) + ((i - numStdSlaves) * lastRowsPerSlave) + lastRowsPerSlave - 1;
            }
            for(int j = 0; j < size; j++) {
                if(array[boundLow][j] && array[boundLow + 1][j]) {
                    unionAB(setArr, sizeArr, size, boundLow, j, boundLow + 1, j);
                }
            }
        }

    if((numSlaves - 1) < numStdSlaves) {
        boundLow = (numSlaves - 1) * stdRowsPerSlave + stdRowsPerSlave - 1;
    } else {
        boundLow = (numStdSlaves * stdRowsPerSlave) + (((numSlaves - 1) - numStdSlaves) * lastRowsPerSlave) + lastRowsPerSlave - 1;
    }
    for(int j = 0; j < size; j++) {
        if(array[boundLow][j] && array[0][j]) {
            // printf("hi\n");
            unionAB(setArr, sizeArr, size, boundLow, j, 0, j);
        }
    }
    for(int j = 0; j < size; j++) {
        if(array[j][boundLow] && array[j][0]) {
            // printf("hi\n");
            unionAB(setArr, sizeArr, size, j, boundLow, j, 0);
        }
    }

    printf("got here 1\n");

    unsigned long long largestClusterSize = findLargestSize(sizeArr, size);

    printf("got here 2\n");

    free(receiveReq);
    destroyArraySite(arrCpy, stdRowsPerSlave);
    destroySetArr(setArr);
    destroySizeArr(sizeArr);

    printf("got here 3\n");

    printf("Largest size: %llu\n", largestClusterSize);

    return largestClusterSize;

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

    char** array;

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

        array = receiveArrayPortion(numMsgs, stdMsgSize, numRows, numCols);

        printf("--------- Slave %d's array: %dx%d--------------\n", rank, numRows, numCols);
        // printLatticeSite(array, numRows, numCols);

        unsigned long long* setArr = createSetArr(numRows, numCols);
        unsigned long long* sizeArr = createSizeArr(numRows, numCols);

        findLargestClusterSiteThread(array, numRows, numCols, setArr, sizeArr, numThreads);

        // for(int i = 0; i < numRows * numCols; i++) {
        //     // printf("Slave\t%d\t%llu\t%llu\n", i, setArr[i], sizeArr[i]);
        //     setArr[i] = i;
        //     sizeArr[i] = i;
        // }

        adjustSetArr(setArr, numRows, numCols, masterRowPos);

        sendArray1dUll(setArr, numMsgs, stdMsgSize, lastMsgSize, TAG_SET_ARR);
        sendArray1dUll(sizeArr, numMsgs, stdMsgSize, lastMsgSize, TAG_SIZE_ARR);

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
