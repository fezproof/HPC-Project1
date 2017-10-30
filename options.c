#include "options.h"

#define NO_MORE_OPTIONS -1
#define OPTLIST "l:P:p:r:t:n:m:M:"

void printUsage(void)
{
    printf("\n\nValid options are:\n"
            "\t-l <s OR b> : type of lattice (site OR bond)\n"
            "\t-P <0-1 float> : chance of allocation\n"
            "\t-p <0, 1, OR 2> : whether percolation does UD, LR, or both\n"
            "\t-r <positive int> : how many runs per size for averaging\n"
            "\t-t <positive int> : number of threads to use\n"
            "\t-n <positive int> : number of node to use\n"
            "\t-m <positive even int> : min lattice size\n"
            "\t-M <positive even int> : max lattice size\n\n");
    exit(EXIT_FAILURE);
}

int readOptions(int argc, char *argv[], OPTIONS *options)
{
    int opt;
    opterr = 0;
    while((opt = getopt(argc, argv, OPTLIST)) != NO_MORE_OPTIONS) {
        switch(opt) {
            case 'l' :
                if (optarg[0] == 's' || optarg[0] == 'b') {
                    options->type = optarg[0];
                } else {
                    argc = -1;
                } break;
            case 'P' :
                if (atof(optarg) >= 0.0 && atof(optarg) <= 1.0) {
                    options->probability = atof(optarg);
                } else {
                    argc = -1;
                } break;
            case 'p' :
                if (atoi(optarg) >= 0 || atoi(optarg) <= 2) {
                    options->perlocationType = atoi(optarg);
                } else {
                    argc = -1;
                } break;
            case 'r' :
                if (atoi(optarg) > 0) {
                    options->runs = atoi(optarg);
                } else {
                    argc = -1;
                } break;
            case 't' :
                if (atoi(optarg) > 0) {
                    options->threadNum = atoi(optarg);
                } else {
                    argc = -1;
                } break;
            case 'n' :
                if (atoi(optarg) > 0) {
                    options->nodeNum = atoi(optarg);
                } else {
                    argc = -1;
                } break;
            case 'm' :
                if (atoi(optarg) > 0 && atoi(optarg) % 2 == 0) {
                    options->minSize = atoi(optarg);
                } else {
                    argc = -1;
                } break;
            case 'M' :
                if (atoi(optarg) >= options->minSize && atoi(optarg) % 2 == 0) {
                    options->maxSize = atoi(optarg);
                } else {
                    argc = -1;
                } break;
            default  :  argc = -1;
        }
        if(argc < 0) {
            fprintf(stderr, "\nError with options at: %c. : %s", optopt, strerror(opterr));
            printUsage();
            exit(EXIT_FAILURE);
        }
    }
    return optind; //the index of the first non-option
}

void printOptions(OPTIONS *options)
{
    printf("\n\n");
    printf("l : %c\n", options->type);
    printf("P : %f\n", options->probability);
    printf("p : %d\n", options->perlocationType);
    printf("r : %d\n", options->runs);
    printf("t : %d\n", options->threadNum);
    printf("n : %d\n", options->nodeNum);
    printf("m : %d\n", options->minSize);
    printf("M : %d\n", options->maxSize);
}

void initialiseOptionsStruct(OPTIONS *options)
{
    options->type = 's';
    options->probability = 0.6;
    options->perlocationType = 2; //-1 indicates to search ALL subdirectories
    options->runs = 1;
    options->threadNum = 4;
    options->nodeNum = 1;
    options->minSize = 16;
    options->maxSize = 8192;
}

OPTIONS *createOptionsStruct(void)
{
    OPTIONS *options = (OPTIONS *)calloc(1, sizeof(OPTIONS));
    if(options == NULL) {
        perror("initialiseOptionsStruct");
        exit(EXIT_FAILURE);
    }
    initialiseOptionsStruct(options);
    return options;
}
