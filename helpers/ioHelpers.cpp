//
// Created by nm on 28.01.2020.
//

#include <iostream>
#include <fstream>

#include "ioHelpers.h"

using namespace std;

void loadFromFile(bool **&w, string const &fileName, int &n, int &m) {
    ifstream in;
    in.open(fileName);
    if (in.fail()) {
        cout << "Can't find or open file: " << fileName << "\n";
        exit(-1);
    }
    in >> n >> m;

    w = allocMatrix(n, m);

    int x;
    for (auto i = 1; i <= n; i++) {
        for (auto j = 1; j <= m; j++) {
            in >> x;
            w[i][j] = (bool) x;
        }
    }
    in.close();
}

void swap(bool **&a, bool **&b) {
    bool **tmp = a;
    a = b;
    b = tmp;
}

bool **allocMatrix(int n, int m, int on_threads) {
    bool **w;
    w = (bool **) calloc(n + 2, sizeof(bool *));

    if (on_threads) {
#pragma omp parallel for num_threads(on_threads)
        for (auto i = 0; i <= n + 1; i++) {
            w[i] = (bool *) calloc(m + 2, sizeof(bool));
        }
    } else {
        for (auto i = 0; i <= n + 1; i++) {
            w[i] = (bool *) calloc(m + 2, sizeof(bool));
        }
    }
    return w;
}

void copyMatrixContent(bool **s, bool **d, int n, int m, int on_threads) {

    if (on_threads) {
#pragma omp parallel for num_threads(on_threads)
        for (auto i = 1; i <= n; i++) {
            for (auto j = 1; j <= m; j++) {
                d[i][j] = s[i][j];
            }
        }

    } else {
        for (auto i = 1; i <= n; i++) {
            for (auto j = 1; j <= m; j++) {
                d[i][j] = s[i][j];
            }
        }
    }
}

bool **cloneMatrix(bool **w, int n, int m, int on_threads) {
    bool **tmp = allocMatrix(n, m);
    copyMatrixContent(w, tmp, n, m, on_threads);
    return tmp;
}


void clear();

void displayMatrix(bool **w, int n, int m, int step, bool clearConsole) {
    if (clearConsole) {
        clear();
    }
    cout << endl;
    if (step != -1) {
        cout << "Step #" << step << "\n\n";
    }
    for (auto i = 1; i <= n; i++) {
        for (auto j = 1; j <= m; j++) {
            cout << " " << (w[i][j] ? "X" : "-") << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

void clear() {
    cout << "\033[2J\033[1;1H";
}


void doInitialWork(bool **&initialWorld, bool **&tmpMatrix, int &n, int &m, int steps_count, int test_count,
                   string &fileName) {
    cout << "fileName: " << fileName << endl;
    loadFromFile(initialWorld, fileName, n, m);

    cout << "size: " << n << "x" << m << endl;

    cout << "tests per type: " << test_count << endl;
    cout << "steps: " << steps_count << endl;

    tmpMatrix = cloneMatrix(initialWorld, n, m);
}
