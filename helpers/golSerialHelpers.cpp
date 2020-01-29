//
// Created by nm on 28.01.2020.
//


#include "golSerialHelpers.h"
#include "golHelpers.h"
#include "ioHelpers.h"
#include <string>

using namespace std;

void
gameOfLifeStepSerial(bool **&v, bool **&customTmp, int n, int m, int startRowOffset, bool skipSwap, int virtualOffset,
                     string const tag) {

    static bool **tmp;
    if (customTmp != nullptr) {
        tmp = customTmp;
    } else {
        if (!tmp) {
            tmp = allocMatrix(n, m);
        }
    }


    for (auto i = 1 + startRowOffset; i < n; i++) {
        if (!tag.empty()) {
//            cout << tag << " processing row " << (virtualOffset != -1 ? (i + virtualOffset) : -1) << endl;
        }
        for (auto j = 1; j < m; j++) {
            updateCell(v, tmp, i, j);
        }
    }
    if (!skipSwap) {
        swap(v, tmp);
    }
//    copyMatrixContent(tmp, v, n, m, startRowOffset);
}
