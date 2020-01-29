//
// Created by nm on 28.01.2020.
//

#ifndef PA_GOLSERIALHELPERS_H
#define PA_GOLSERIALHELPERS_H

#include <vector>
#include <string>

using namespace std;

void
gameOfLifeStepSerial(bool **&w, bool **&customTmp, int n, int m,
                     int startRowOffset = 0,
                     bool skipSwap = false,
                     int virtualOffset = -1,
                     string tag = nullptr);

#endif //PA_GOLSERIALHELPERS_H
