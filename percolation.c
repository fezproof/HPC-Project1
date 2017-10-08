#include "percolation.h"

int percolateBond(BONDSITE** arr, int size, int type) {
    if (type == 0) {
        return dfsUpDownBond(arr, size);
    } else if (type == 1) {
        return dfsLeftRightBond(arr, size);
    } else if (type == 2) {
        return (dfsLeftRightBond(arr, size) && dfsUpDownBond(arr, size));
    }
    return 0;
}

int percolateBondThread(BONDSITE** arr, int size, int type) {
    if (type == 0) {
        return dfsUpDownBondThread(arr, size);
    } else if (type == 1) {
        return dfsLeftRightBondThread(arr, size);
    } else if (type == 2) {
        return (dfsLeftRightBondThread(arr, size) && dfsUpDownBondThread(arr, size));
    }
    return 0;
}

int percolateSite(char** arr, int size, int type) {
    if (type == 0) {
        return dfsUpDownSite(arr, size);
    } else if (type == 1) {
        return dfsLeftRightSite(arr, size);
    } else if (type == 2) {
        return (dfsLeftRightSite(arr, size) && dfsUpDownSite(arr, size));
    }
    return 0;
}

int percolateSiteThread(char** arr, int size, int type) {
    if (type == 0) {
        return dfsUpDownSiteThread(arr, size);
    } else if (type == 1) {
        return dfsLeftRightSiteThread(arr, size);
    } else if (type == 2) {
        return (dfsLeftRightSiteThread(arr, size) && dfsUpDownSiteThread(arr, size));
    }
    return 0;
}
