//
// Created by nm on 28.01.2020.
//

#ifndef PA_GOLHELPERS_H
#define PA_GOLHELPERS_H

#include <vector>

using namespace std;

void addPadding(vector<vector<bool>> &vv, int up, int right, int bottom, int left);

void updateCell(vector<vector<bool>> &in, vector<vector<bool>> &out, int i, int j, int n, int m);

int countAliveCells(vector<vector<bool>> &v);

#endif //PA_GOLHELPERS_H
