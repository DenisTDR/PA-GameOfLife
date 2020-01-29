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
