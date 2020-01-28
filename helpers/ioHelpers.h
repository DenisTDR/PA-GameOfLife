//
// Created by nm on 28.01.2020.
//

#ifndef PA_IOHELPERS_H
#define PA_IOHELPERS_H

#include <vector>
#include <iostream>
#include <fstream>

using namespace std;
void displayMatrix(vector<vector<bool>> &vv, int step = -1, bool clearConsole = true);
void loadFromFile(vector<vector<bool>> &vv, string const &fileName);

#endif //PA_IOHELPERS_H
