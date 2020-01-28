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

char countAliveNeighbours(vector<vector<bool>> &vv, int i, int j, int n, int m) {
    char alive = 0;
    int x, y;
    for (auto &neighboursOffset : neighboursOffsets) {
        x = i + neighboursOffset[0];
        y = j + neighboursOffset[1];

        //check out of range
        if (x < 0 || y < 0 || x >= n || y >= m) continue;

        if (vv[x][y]) {
            alive++;
        }
    }

    return alive;
}

bool calcNewState(bool oldState, int aliveNeighs) {
    return aliveNeighs == 3 || (oldState && aliveNeighs == 2);
}

void updateCell(vector<vector<bool>> &in, vector<vector<bool>> &out, int i, int j, int n, int m) {
    char aliveNeighbours = countAliveNeighbours(in, i, j, n, m);
    out[i][j] = calcNewState(in[i][j], aliveNeighbours);
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
    itv = vv.begin();
    vv.insert(itv, up, vector<bool>(vv[0].size(), false));

    itv = vv.end();
    vv.insert(itv, bottom, vector<bool>(vv[0].size(), false));
}

int countAliveCells(vector<vector<bool>> &v) {
    int count = 0;

    for (auto &i : v) {
        for (auto &&j : i) {
            count = count + j;
        }
    }
    return count;
}