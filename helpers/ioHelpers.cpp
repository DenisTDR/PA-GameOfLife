//
// Created by nm on 28.01.2020.
//

#include <iostream>
#include <fstream>
#include <sstream>

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

void copyMatrixContent(bool **s, bool **d, int n, int m, int on_threads, int startRowOffset) {

    if (on_threads) {
#pragma omp parallel for num_threads(on_threads)
        for (auto i = 1 + startRowOffset; i <= n; i++) {
            for (auto j = 1; j <= m; j++) {
                d[i][j] = s[i][j];
            }
        }

    } else {
        for (auto i = 1 + startRowOffset; i <= n; i++) {
            for (auto j = 1; j <= m; j++) {
                d[i][j] = s[i][j];
            }
        }
    }
}

bool **cloneMatrix(bool **w, int n, int m, int on_threads) {
    bool **tmp = allocMatrix(n, m);
//    cout << "cloning matrix" << endl;
    copyMatrixContent(w, tmp, n, m, on_threads);
    return tmp;
}

void clearMatrix(bool **w, int n, int m) {
    for (auto i = 1; i <= n; i++) {
        for (auto j = 1; j <= m; j++) {
            w[i][j] = false;
        }
    }
}

void clear();

void displayMatrix(bool **w, int n, int m, int step, bool clearConsole) {
    if (clearConsole) {
        clear();
    }
    stringstream ss;
    ss << endl;
    if (step != -1) {
        ss << "Step #" << step << "\n\n";
    }
    for (auto i = 1; i <= n; i++) {
        for (auto j = 1; j <= m; j++) {
            ss << " " << (w[i][j] ? "X" : "-") << " ";
        }
        ss << "\n";
    }
    ss << "\n";
    cout << ss.str();
}

void clear() {
    cout << "\033[2J\033[1;1H";
}


void doInitialWork(bool **&initialWorld, int &n, int &m, int steps_count, int test_count,
                   string &fileName) {
    cout << "fileName: " << fileName << endl;
    loadFromFile(initialWorld, fileName, n, m);

    cout << "size: " << n << "x" << m << endl;

    cout << "tests per type: " << test_count << endl;
    cout << "steps: " << steps_count << endl;
}

void writeBigMatrix(bool **&w, int n, int m, string fileName) {
    ofstream out;
    out.open(fileName);
    if (out.fail()) {
        cout << "can't open file: " << fileName << endl;
        exit(-1);
    }

    out << n << " " << m << "\n";

    for (auto i = 1; i <= n; i++) {
        for (auto j = 1; j <= m; j++) {
            out << w[i][j] << " ";
        }
        out << "\n";
    }
    out.flush();
    out.close();
}
