#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char    type;
    float   probability;
    short   perlocationType;
    int     runs;
    int     minThreadNum;
    int     maxThreadNum;
    int     minNodeNum;
    int     maxNodeNum;
    int     minSize;
    int     maxSize;
} OPTIONS;

extern void printUsage(void);
extern int  readOptions(int argc, char *argv[], OPTIONS *options);
extern void printOptions(OPTIONS *options);
extern OPTIONS *createOptionsStruct(void);
