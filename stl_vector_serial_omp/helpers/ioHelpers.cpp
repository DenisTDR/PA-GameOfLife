//
// Created by nm on 28.01.2020.
//

#include <vector>
#include <iostream>
#include <fstream>

#include "ioHelpers.h"

using namespace std;

void loadFromFile(vector<vector<bool>> &vv, string const &fileName) {
    ifstream in;
    in.open(fileName);
    if (in.fail()) {
        cout << "Can't find or open file: " << fileName << "\n";
        exit(-1);
    }
    int n, m;
    in >> n >> m;
    vv.resize(n, vector<bool>(m));
    for (auto i = 0; i < n; i++) {
        for (auto j = 0; j < m; j++) {
            bool x;
            in >> x;
            vv[i][j] = x;
        }
    }
    in.close();
}

void clear();

void displayMatrix(vector<vector<bool>> &vv, int step, bool clearConsole) {
    if (clearConsole) {
        clear();
    }
    cout << endl;
    if (step != -1) {
        cout << "Step #" << step << "\n\n";
    }
    for (auto &i : vv) {
        for (bool j : i) {
            cout << " " << (j ? "X" : "-") << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

void clear() {
    cout << "\033[2J\033[1;1H";
}