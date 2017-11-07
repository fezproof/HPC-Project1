#include "node.h"

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

MPI_Datatype MPI_BONDSITE; //declare custom MPI struct

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
        MPI_Send(info, INFO_SIZE, MPI_INT, i, TAG_INFO, MPI_COMM_WORLD);
    }
}

void sendArrToSlavesSite(char** array, int* rowOffset, int firstSlave, int lastSlave, int numMsgs, int stdMsgSize, int lastMsgSize, int numCols, MPI_Request* req)
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

void sendArrToSlavesBond(BONDSITE** array, int* rowOffset, int firstSlave, int lastSlave, int numMsgs, int stdMsgSize, int lastMsgSize, int numCols, MPI_Request* req)
{
    // MPI_Datatype mpi_bondsite;
    // MPI_Type_contiguous(1, MPI_UNSIGNED, &mpi_bondsite);
    for(int i = firstSlave; i < lastSlave; i++) {

        for(int j = 0; j < numMsgs-1; j++) {
            MPI_Isend(&(array[*rowOffset][0]), stdMsgSize, MPI_UNSIGNED, i, TAG_ARR, MPI_COMM_WORLD, req);
            (*rowOffset) += stdMsgSize / numCols;
        }
        if(lastMsgSize == 0) {
            MPI_Isend(&(array[*rowOffset][0]), stdMsgSize, MPI_UNSIGNED, i, TAG_ARR, MPI_COMM_WORLD, req);
            (*rowOffset) += stdMsgSize / numCols;
        } else {
            MPI_Isend(&(array[*rowOffset][0]), lastMsgSize, MPI_UNSIGNED, i, TAG_ARR, MPI_COMM_WORLD, req);
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

void** receiveArrayPortion(int numMsgs, int stdMsgSize, int numRows, int numCols, char type)
{
    void** array;

    if(type == 's') {
        array = (void**) createBlankLatticeSite(numRows, numCols);
    } else {
        array = (void**) createBlankLatticeBond(numRows, numCols);
    }

    MPI_Status* status = malloc(numMsgs * sizeof(MPI_Status));
    MPI_Request* req = malloc(numMsgs * sizeof(MPI_Request));

    int rowOffset = 0;
    int curMsgNum = 0;

    if(type == 's') {
        char** cArr = (char**) array;
        for(int i = 0; i < numMsgs; i++) {
            MPI_Irecv(&cArr[rowOffset][0], stdMsgSize, MPI_CHAR, 0, TAG_ARR, MPI_COMM_WORLD, &req[curMsgNum]);
            rowOffset += stdMsgSize / numCols;
            curMsgNum++;
        }
    } else {
        BONDSITE** bArr = (BONDSITE**) array;
        // MPI_Datatype mpi_bondsite;
        // MPI_Type_contiguous(1, MPI_UNSIGNED, &mpi_bondsite);
        for(int i = 0; i < numMsgs; i++) {
            MPI_Irecv(&bArr[rowOffset][0], stdMsgSize, MPI_UNSIGNED, 0, TAG_ARR, MPI_COMM_WORLD, &req[curMsgNum]);
            rowOffset += stdMsgSize / numCols;
            curMsgNum++;
        }
    }

    MPI_Waitall(numMsgs, req, status);

    return array;
}

void adjustSetArr(unsigned long long* setArr, int numRows, int numCols, int masterRowPos)
{
    unsigned long long setSize = (unsigned long long) numRows * (unsigned long long) numCols;
    for(unsigned long long i = 0; i < setSize; i++) {
        setArr[i] += masterRowPos * numCols;
    }
}

void stitchNodes(void** array, char latticeType, unsigned long long* sizeArr, unsigned long long* setArr, int size, int numSlaves, int numStdSlaves, int stdRowsPerSlave, int lastRowsPerSlave) {
    int boundLow;

    for(int i = 0; i < numSlaves - 1; i++) {
        if(i < numStdSlaves) {
            boundLow = i * stdRowsPerSlave + stdRowsPerSlave - 1;
        } else {
            boundLow = (numStdSlaves * stdRowsPerSlave) + ((i - numStdSlaves) * lastRowsPerSlave) + lastRowsPerSlave - 1;
        }
        if(latticeType == 's') {
            for(int j = 0; j < size; j++) {
                if(((char**)array)[boundLow][j] && ((char**)array)[boundLow + 1][j]) {
                    unionAB(setArr, sizeArr, size, boundLow, j, boundLow + 1, j);
                }
            }
        } else {
            for(int j = 0; j < size; j++) {
                if(((BONDSITE**)array)[boundLow][j].down) {
                    unionAB(setArr, sizeArr, size, boundLow, j, boundLow + 1, j);
                }
            }
        }
    }
}

int checkPerculationSite(char** array, int size, unsigned long long* setArr, unsigned long long* sizeArr, int type) {
    if (type == 0) {
        // Up to Down perculation

        unsigned long long* setArrUD = createSizeArr(size, size);
        memcpy(setArrUD, setArr, (unsigned long long) size * (unsigned long long) size);
        unsigned long long* sizeArrUD = createSizeArr(size, size);
        memcpy(sizeArrUD, sizeArr, (unsigned long long) size * (unsigned long long) size);

        int percolates = 0;

        for(int j = 0; j < size; j++) {
            if(array[j][size - 1] && array[j][0]) {
                unionAB(setArrUD, sizeArrUD, size, j, size - 1, j, 0);
            }
        }

        #pragma omp parallel for
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    if (!percolates && array[0][i] && array[size - 1][j] && find(setArrUD, size, 0, i, size - 1, j)) {
                        #pragma omp atomic write
                            percolates = 1;
                    }
                }
            }

        if(percolates) {
            free(setArrUD);
            free(sizeArrUD);
            return 1;
        }

        free(setArrUD);
        free(sizeArrUD);
    } else if (type == 1) {
        // Left to Right perculation

        unsigned long long* setArrLR = createSizeArr(size, size);
        memcpy(setArrLR, setArr, (unsigned long long) size * (unsigned long long) size);
        unsigned long long* sizeArrLR = createSizeArr(size, size);
        memcpy(sizeArrLR, sizeArr, (unsigned long long) size * (unsigned long long) size);

        int percolates = 0;

        for(int j = 0; j < size; j++) {
            if(array[size - 1][j] && array[0][j]) {
                unionAB(setArrLR, sizeArrLR, size, size - 1, j, 0, j);
            }
        }

        #pragma omp parallel for
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    if (!percolates && array[i][0] && array[j][size - 1] && find(setArrLR, size, i, 0, j, size - 1)) {
                        #pragma omp atomic write
                            percolates = 1;
                    }
                }
            }


        if(percolates) {
            free(setArrLR);
            free(sizeArrLR);
            return 1;
        }
        free(setArrLR);
        free(sizeArrLR);
    } else if (type == 2) {
        // Left to Right and Up to Down perculation
        return (checkPerculationSite(array, size, setArr, sizeArr, 0)
            && checkPerculationSite(array, size, setArr, sizeArr, 1));
    } else {
        printf("Type argument is wrong\n");
        return 0;
    }

    return 0;
}

int checkPerculationBond(BONDSITE** array, int size, unsigned long long* setArr, unsigned long long* sizeArr, int type) {
    if (type == 0) {
        // Up to Down perculation

        unsigned long long* setArrUD = createSizeArr(size, size);
        memcpy(setArrUD, setArr, (unsigned long long) size * (unsigned long long) size);
        unsigned long long* sizeArrUD = createSizeArr(size, size);
        memcpy(sizeArrUD, sizeArr, (unsigned long long) size * (unsigned long long) size);

        int percolates = 0;

        for(int j = 0; j < size; j++) {
            if(array[j][0].left) {
                unionAB(setArrUD, sizeArrUD, size, j, size - 1, j, 0);
            }
        }

        #pragma omp parallel for
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    if (!percolates && array[0][i].up && find(setArrUD, size, 0, i, size - 1, j)) {
                        #pragma omp atomic write
                            percolates = 1;
                    }
                }
            }



        if(percolates) {
            free(setArrUD);
            free(sizeArrUD);
            return 1;
        }

        free(setArrUD);
        free(sizeArrUD);
    } else if (type == 1) {
        // Left to Right perculation

        unsigned long long* setArrLR = createSizeArr(size, size);
        memcpy(setArrLR, setArr, (unsigned long long) size * (unsigned long long) size);
        unsigned long long* sizeArrLR = createSizeArr(size, size);
        memcpy(sizeArrLR, sizeArr, (unsigned long long) size * (unsigned long long) size);

        int percolates = 0;

        for(int j = 0; j < size; j++) {
            if(array[0][j].up) {
                unionAB(setArrLR, sizeArrLR, size, size - 1, j, 0, j);
            }
        }

        #pragma omp parallel for
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    if (!percolates && array[i][0].left && find(setArrLR, size, i, 0, j, size - 1)) {
                        #pragma omp atomic write
                            percolates = 1;
                    }
                }
            }

        if(percolates) {
            free(setArrLR);
            free(sizeArrLR);
            return 1;
        }

        free(setArrLR);
        free(sizeArrLR);
    } else if (type == 2) {
        // Left to Right and Up to Down perculation
        return (checkPerculationBond(array, size, setArr, sizeArr, 0)
            && checkPerculationBond(array, size, setArr, sizeArr, 1));
    } else {
        printf("Type argument is wrong\n");
        return 0;
    }

    return 0;
}

unsigned long long clusterMaster(void** array, char latticeType, int size, int numSlaves, int numThreads, int* perc, int percType)
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
    if(latticeType == 's') {
        sendArrToSlavesSite((char**)array, &rowOffet, 1, numStdSlaves, info1[NUM_MSGS_INDEX], info1[STD_MSG_SIZE_INDEX], info1[LAST_MSG_SIZE_INDEX], numCols, &sendReq);
    } else {
        sendArrToSlavesBond((BONDSITE**)array, &rowOffet, 1, numStdSlaves, info1[NUM_MSGS_INDEX], info1[STD_MSG_SIZE_INDEX], info1[LAST_MSG_SIZE_INDEX], numCols, &sendReq);
    }

    //---------------------------- Final procs ----------------------------------

    determineSizeOfMessages(lastRowsPerSlave, numCols, &info2[STD_MSG_SIZE_INDEX], &info2[LAST_MSG_SIZE_INDEX], &info2[NUM_MSGS_INDEX]);
    sendInfoToSlaves(info2, numStdSlaves, numSlaves, numStdSlaves*stdRowsPerSlave);
    if(latticeType == 's') {
        sendArrToSlavesSite((char**)array, &rowOffet, numStdSlaves, numSlaves, info2[NUM_MSGS_INDEX], info2[STD_MSG_SIZE_INDEX], info2[LAST_MSG_SIZE_INDEX], numCols, &sendReq);
    } else {
        sendArrToSlavesBond((BONDSITE**)array, &rowOffet, numStdSlaves, numSlaves, info2[NUM_MSGS_INDEX], info2[STD_MSG_SIZE_INDEX], info2[LAST_MSG_SIZE_INDEX], numCols, &sendReq);
    }
    //--------------- Process the first segment of the array -------------------

    // printf("--------- Master arr %dx%d--------------\n", stdRowsPerSlave, numCols);
    // if(latticeType == 's') printLatticeSite((char**)array, stdRowsPerSlave, numCols);
    // else printLatticeBond((BONDSITE**)array, stdRowsPerSlave, numCols);

    unsigned long long* setArr = createSetArr(size, size);
    unsigned long long* sizeArr = createSizeArr(size, size);

    if(latticeType == 's') {
        char** arrCpy;
        arrCpy = copyLatticeSite((char**)array, stdRowsPerSlave, numCols);
        findLargestClusterSiteThread(arrCpy, stdRowsPerSlave, numCols, setArr, sizeArr, numThreads);
        destroyArray((void**)arrCpy, stdRowsPerSlave);
    } else {
        BONDSITE** arrCpy;
        arrCpy = copyLatticeBond((BONDSITE**)array, stdRowsPerSlave, numCols);
        findLargestClusterBondThread(arrCpy, stdRowsPerSlave, numCols, setArr, sizeArr, numThreads);
        destroyArray((void**)arrCpy, stdRowsPerSlave);
    }

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

    MPI_Waitall(numMsgsToReceive, receiveReq, status);

    stitchNodes(array, latticeType, sizeArr, setArr, size, numSlaves, numStdSlaves, stdRowsPerSlave, lastRowsPerSlave);

    if(latticeType == 's') {
        *perc = checkPerculationSite((char**)array, size, setArr, sizeArr, 2);

        for(int j = 0; j < size; j++) {
            if(((char**)array)[size - 1][j] && ((char**)array)[0][j]) {
                unionAB(setArr, sizeArr, size, size - 1, j, 0, j);
            }
        }
        for(int j = 0; j < size; j++) {
            if(((char**)array)[j][size - 1] && ((char**)array)[j][0]) {
                unionAB(setArr, sizeArr, size, j, size - 1, j, 0);
            }
        }

    } else {
        *perc = checkPerculationBond((BONDSITE**)array, size, setArr, sizeArr, 2);

        for(int j = 0; j < size; j++) {
            if(((BONDSITE**)array)[size - 1][j].down) {
                unionAB(setArr, sizeArr, size, size - 1, j, 0, j);
            }
        }
        for(int j = 0; j < size; j++) {
            if(((BONDSITE**)array)[j][size - 1].right) {
                unionAB(setArr, sizeArr, size, j, size - 1, j, 0);
            }
        }

    }

    unsigned long long largestClusterSize = findLargestSize(sizeArr, size);

    free(receiveReq);
    destroySetArr(setArr);
    destroySizeArr(sizeArr);

    return largestClusterSize;

}

void clusterSlave(char latticeType)
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

    void** array;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    while(1) {
        MPI_Recv(info, INFO_SIZE, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if (status.MPI_TAG == TAG_TERMINATE) {
            // printf("terminating: %d\n", rank);
            break;
        }

        numRows = info[NUM_ROWS_INDEX];
        numCols = info[NUM_COLS_INDEX];
        stdMsgSize = info[STD_MSG_SIZE_INDEX];
        lastMsgSize = info[LAST_MSG_SIZE_INDEX];
        numMsgs = info[NUM_MSGS_INDEX];
        numThreads = info[THREADS_INDEX];
        masterRowPos = info[MASTER_ROW_POS_INDEX];

        omp_set_num_threads(numThreads);

        array = receiveArrayPortion(numMsgs, stdMsgSize, numRows, numCols, latticeType);

        // printf("--------- Slave %d's array: %dx%d--------------\n", rank, numRows, numCols);
        // if(latticeType == 's') printLatticeSite((char**)array, numRows, numCols);
        // else printLatticeBond((BONDSITE**)array, numRows, numCols);

        unsigned long long* setArr = createSetArr(numRows, numCols);
        unsigned long long* sizeArr = createSizeArr(numRows, numCols);

        if(latticeType == 's') {
            char** cArr = (char**) array;
            findLargestClusterSiteThread(cArr, numRows, numCols, setArr, sizeArr, numThreads);
        } else {
            BONDSITE** bArr = (BONDSITE**) array;
            findLargestClusterBondThread(bArr, numRows, numCols, setArr, sizeArr, numThreads);
        }

        adjustSetArr(setArr, numRows, numCols, masterRowPos);

        sendArray1dUll(setArr, numMsgs, stdMsgSize, lastMsgSize, TAG_SET_ARR);
        sendArray1dUll(sizeArr, numMsgs, stdMsgSize, lastMsgSize, TAG_SIZE_ARR);

        destroySetArr(setArr);
        destroySizeArr(sizeArr);
        destroyArray(array, numRows);
    }
}

unsigned long long clusterBondMaster(BONDSITE** array, int size, int numSlaves, int numThreads, int*perc, int percType)
{
    return 0;
}
