#ifndef BOND_H
#define BOND_H

typedef struct bondsite BONDSITE;

struct bondsite {
    unsigned int up : 2, down : 2, left : 2, right : 2;
};

#endif
