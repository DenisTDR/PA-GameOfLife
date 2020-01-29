//
// Created by nm on 28.01.2020.
//

#ifndef PA_GOLOPENMPHELPERS_H
#define PA_GOLOPENMPHELPERS_H

#include <vector>
#include <string>

using namespace std;

void gameOfLifeStepOpenMp(bool **&v, int n, int m, int threads);

void
gameOfLifeOpenMp2(bool **&w, bool **&customTmp, int n, int m,
                  int startRowOffset = 0,
                  bool skipSwap = false,
                  int virtualOffset = -1,
                  string tag = nullptr,
                  int threads = 0);

#endif //PA_GOLOPENMPHELPERS_H