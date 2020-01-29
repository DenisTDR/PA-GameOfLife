//
// Created by nm on 28.01.2020.
//


#include "golSerialHelpers.h"
#include "golHelpers.h"
#include "ioHelpers.h"

using namespace std;

void gameOfLifeStepSerial(bool **&v, int n, int m) {

    static bool **tmp;
    if(!tmp) {
        tmp = allocMatrix(n, m);
    }

    for (auto i = 1; i < n; i++) {
        for (auto j = 1; j < m; j++) {
            updateCell(v, tmp, i, j);
        }
    }
    swap(v, tmp);
}
