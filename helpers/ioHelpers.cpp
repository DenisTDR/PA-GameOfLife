//
// Created by nm on 28.01.2020.
//

#include <iostream>
#include <fstream>

#include "ioHelpers.h"

using namespace std;

void loadFromFile(char **&w, string const &fileName, int &n, int &m) {
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
            w[i][j] = (char) x;
        }
    }
    in.close();
}

void swap(char **&a, char **&b) {
    char **tmp = a;
    a = b;
    b = tmp;
}

char **allocMatrix(int n, int m, int on_threads) {
    char **w;
    w = (char **) calloc(n + 2, sizeof(char *));

    if (on_threads) {
#pragma omp parallel for num_threads(on_threads)
        for (auto i = 0; i <= n + 1; i++) {
            w[i] = (char *) calloc(m + 2, sizeof(char));
        }
    } else {
        for (auto i = 0; i <= n + 1; i++) {
            w[i] = (char *) calloc(m + 2, sizeof(char));
        }
    }
    return w;
}

void copyMatrixContent(char **s, char **d, int n, int m, int on_threads) {

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

char **cloneMatrix(char **w, int n, int m, int on_threads) {
    char **tmp = allocMatrix(n, m);
    copyMatrixContent(w, tmp, n, m, on_threads);
    return tmp;
}


void clear();

void displayMatrix(char **w, int n, int m, int step, bool clearConsole) {
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