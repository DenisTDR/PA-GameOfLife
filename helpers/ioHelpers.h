//
// Created by nm on 28.01.2020.
//

#ifndef PA_IOHELPERS_H
#define PA_IOHELPERS_H

#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

void displayMatrix(bool **w, int n, int m, int step = -1, bool clearConsole = true);

void loadFromFile(bool **&w, string const &fileName, int &n, int &m);

void swap(bool **&a, bool **&b);

bool **cloneMatrix(bool **w, int n, int m, int on_threads = 0);

void copyMatrixContent(bool **s, bool **d, int n, int m, int on_threads = 0, int startRowOffset = 0);

bool **allocMatrix(int n, int m, int on_threads = 0);

void doInitialWork(bool **&initialWorld, int &n, int &m, int steps_count, int test_count, string &fileName);

void clearMatrix(bool **w, int n, int m);

void writeBigMatrix(bool **&w, int n, int m, string fileName);


#endif //PA_IOHELPERS_H
