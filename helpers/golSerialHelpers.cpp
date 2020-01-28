//
// Created by nm on 28.01.2020.
//


#include "golSerialHelpers.h"
#include "golHelpers.h"

using namespace std;

vector<vector<bool>> tmp;

void gameOfLifeStepSerial(vector<vector<bool>> &v) {

    tmp.resize(v.size(), vector<bool>(v[1].size()));

    int n = (int) v.size() - 1;
    int m = (int) v[1].size() - 1;

    for (auto i = 1; i < n; i++) {
        for (auto j = 1; j < m; j++) {
            updateCell(v, tmp, i, j, n, m);
        }
    }
//    double tbegin = omp_get_wtime();
//    v.clear();
//    copy(tmp.begin(), tmp.end(), back_inserter(v));

    v = tmp;
//    v.swap(tmp);
//    double time_spent = omp_get_wtime() - tbegin;
//    cout << "time spent for copy: " << time_spent << endl;
}
