#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bond.h"

extern char** createLatticeSite(int size, double chance);
extern BONDSITE** createLatticeBond(int size, double chance);
extern void destroyArraySite(char** arr, int size);
extern void destroyArrayBond(BONDSITE** arr, int size);
extern char** copyLatticeSite(char** src, int size);
extern char** copyLatticeSiteThread(char** src, int size);
extern BONDSITE** copyLatticeBond(BONDSITE** src, int size);
extern BONDSITE** copyLatticeBondThread(BONDSITE** src, int size);


extern void printLatticeSite(char** lattice, int size, char** vertices);
extern void printLatticeBond(BONDSITE** lattice, int size, char** vertices);
