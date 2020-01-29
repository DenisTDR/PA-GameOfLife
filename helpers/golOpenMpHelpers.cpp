//
// Created by nm on 28.01.2020.
//
#include <omp.h>

#include "golOpenMpHelpers.h"
#include "golHelpers.h"
#include "ioHelpers.h"


using namespace std;


void gameOfLifeStepOpenMp(bool **&v, int n, int m, int threads) {
    static bool **tmp;
    if (!tmp) {
        tmp = allocMatrix(n, m, threads);
    }

#pragma omp parallel for num_threads(threads)
    for (auto i = 1; i < n; i++) {
        for (auto j = 1; j < m; j++) {
            updateCell(v, tmp, i, j);
        }
    }
    swap(v, tmp);
}


void gameOfLifeOpenMp2(bool **&v, bool **&customTmp, int n, int m, int startRowOffset, bool skipSwap, int virtualOffset,
                       string const tag, int threads) {

    static bool **tmp;
    if (customTmp != nullptr) {
        tmp = customTmp;
    } else {
        if (!tmp) {
            tmp = allocMatrix(n, m);
        }
    }


#pragma omp parallel for num_threads(threads)
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
