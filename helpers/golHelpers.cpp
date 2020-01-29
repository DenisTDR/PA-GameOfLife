//
// Created by nm on 28.01.2020.
//

#include "golHelpers.h"

int neighboursOffsets[][2] = {{-1, -1},
                              {-1, 0},
                              {-1, 1},
                              {0,  -1},
                              {0,  1},
                              {1,  -1},
                              {1,  0},
                              {1,  1}};

char countAliveNeighbours(bool **vv, int i, int j) {
    char alive = 0;
    for (auto &neighboursOffset : neighboursOffsets) {
        if (vv[i + neighboursOffset[0]][j + neighboursOffset[1]]) {
            alive++;
        }
    }
    return alive;
}

bool calcNewState(bool oldState, int aliveNeighs) {
    return aliveNeighs == 3 || (oldState && aliveNeighs == 2);
}

void updateCell(bool **in, bool **out, int i, int j) {
    if (calcNewState(in[i][j], countAliveNeighbours(in, i, j)) ^ out[i][j]) {
        out[i][j] ^= 1u;
    }
}


void addPadding(vector<vector<bool>> &vv, int up, int right, int bottom, int left) {

    for (auto &i : vv) {
        auto it = i.begin();
        if (left) {
            i.insert(it, left, false);
        }
        if (right) {
            it = i.end();
            i.insert(it, right, false);
        }
    }

    auto itv = vv.begin();
    vv.insert(itv, up, vector<bool>(vv[0].size(), false));

    itv = vv.end();
    vv.insert(itv, bottom, vector<bool>(vv[0].size(), false));
}

int countAliveCells(bool **w, int n, int m) {
    int count = 0;
    for (auto i = 1; i <= n; i++) {
        for (auto j = 1; j <= m; j++) {
            if (w[i][j]) {
                count++;
            }
        }
    }
    return count;
}