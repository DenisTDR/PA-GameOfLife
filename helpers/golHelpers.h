//
// Created by nm on 28.01.2020.
//

#ifndef PA_GOLHELPERS_H
#define PA_GOLHELPERS_H

#include <vector>

using namespace std;

void addPadding(vector<vector<bool>> &vv, int up, int right, int bottom, int left);

void updateCell(bool **in, bool **out, int i, int j);

int countAliveCells(bool **w, int n, int m);

#endif //PA_GOLHELPERS_H
