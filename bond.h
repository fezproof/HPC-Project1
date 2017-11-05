#include <mpi.h>

#ifndef BOND_H
#define BOND_H

typedef struct bondsite BONDSITE;

struct bondsite {
    unsigned int up : 1, down : 1, left : 1, right : 1, seen : 1;
};

#endif
