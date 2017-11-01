#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bond.h"

extern char** createLatticeSite(int size, double chance);
extern BONDSITE** createLatticeBond(int size, double chance);
extern void destroyArraySite(char** arr, int size);
extern void destroyArrayBond(BONDSITE** arr, int size);
extern char** copyLatticeSite(char** src, int numRows, int numCols);
extern char** copyLatticeSiteThread(char** src, int size);
extern BONDSITE** copyLatticeBond(BONDSITE** src, int size);
extern BONDSITE** copyLatticeBondThread(BONDSITE** src, int size);
extern char** createSubLatticeSite(int numRows, int numCols);
extern BONDSITE** createSubLatticeBond(int numRows, int numCols);


extern void printLatticeSite(char** lattice, int size);
extern void printLatticeBond(BONDSITE** lattice, int size);
