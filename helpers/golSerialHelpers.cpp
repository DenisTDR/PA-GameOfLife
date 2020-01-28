//
// Created by nm on 28.01.2020.
//

#include <vector>

#include "golSerialHelpers.h"
#include "golHelpers.h"

using namespace std;

void gameOfLifeStep(vector<vector<bool>> &v) {
    vector<vector<bool>> tmp(v.size(), vector<bool>(v[0].size()));

    int n = v.size();
    int m = v[0].size();

    for (auto i = 0; i < n; i++) {
        for (auto j = 0; j < m; j++) {
            char aliveNeighbours = countAliveNeighbours(v, i, j, n, m);
            tmp[i][j] = calcNewState(v[i][j], aliveNeighbours);
        }
    }

    v = tmp;
}
