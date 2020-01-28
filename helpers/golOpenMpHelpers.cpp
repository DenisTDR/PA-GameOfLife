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

    int n = v.size();
    int m = v[0].size();

    #pragma omp parallel for num_threads(threads)
    for (auto i = 0; i < n; i++) {
        for (auto j = 0; j < m; j++) {
            updateCell(v, tmpOpenMp, i, j, n, m);
        }
    }
    v = tmpOpenMp;
}
