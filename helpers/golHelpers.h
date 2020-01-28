//
// Created by nm on 28.01.2020.
//

#ifndef PA_GOLHELPERS_H
#define PA_GOLHELPERS_H

#include <vector>

using namespace std;

char countAliveNeighbours(vector<vector<bool>> &vv, int i, int j, int n, int m);

bool calcNewState(bool oldState, int aliveNeighs);

void addPadding(vector<vector<bool>> &vv, int up, int right, int bottom, int left);

#endif //PA_GOLHELPERS_H
