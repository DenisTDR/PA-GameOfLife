//
// Created by nm on 28.01.2020.
//


#include "golSerialHelpers.h"
#include "golHelpers.h"

using namespace std;

vector<vector<bool>> tmp;

void gameOfLifeStepSerial(vector<vector<bool>> &v) {

    tmp.resize(v.size(), vector<bool>(v[0].size()));

    int n = v.size();
    int m = v[0].size();

    for (auto i = 0; i < n; i++) {
        for (auto j = 0; j < m; j++) {
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
