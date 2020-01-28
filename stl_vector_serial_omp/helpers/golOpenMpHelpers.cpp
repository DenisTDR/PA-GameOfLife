//
// Created by nm on 28.01.2020.
//
#include <omp.h>

#include "golOpenMpHelpers.h"
#include "golHelpers.h"


using namespace std;

vector<vector<bool>> tmpOpenMp;

void gameOfLifeStepOpenMp(vector<vector<bool>> &v, int threads) {
    tmpOpenMp.resize(v.size(), vector<bool>(v[0].size()));

    int n = (int) v.size() - 1;
    int m = (int) v[1].size() - 1;

    #pragma omp parallel for num_threads(threads)
    for (auto i = 1; i < n; i++) {
        for (auto j = 1; j < m; j++) {
            updateCell(v, tmpOpenMp, i, j, n, m);
        }
    }
    v = tmpOpenMp;
}
