#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bond.h"

extern char** createLatticeSite(int size, double chance);
extern BONDSITE** createLatticeBond(int size, double chance);
extern char** copyLatticeSite(char** src, int numRows, int numCols);
extern BONDSITE** copyLatticeBond(BONDSITE** src, int numRows, int numCols);
extern char** createBlankLatticeSite(int numRows, int numCols);
extern BONDSITE** createBlankLatticeBond(int numRows, int numCols);

extern void destroyArray(void** arr, int size);

extern void printLatticeSite(char** lattice, int numRows, int numCols);
extern void printLatticeBond(BONDSITE** lattice, int numRows, int numCols);
