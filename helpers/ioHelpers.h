//
// Created by nm on 28.01.2020.
//

#ifndef PA_IOHELPERS_H
#define PA_IOHELPERS_H

#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

void displayMatrix(char **w, int n, int m, int step = -1, bool clearConsole = true);

void loadFromFile(char **&w, string const &fileName, int &n, int &m);

void swap(char **&a, char **&b);

char **cloneMatrix(char **w, int n, int m, int on_threads = 0);

void copyMatrixContent(char **s, char **d, int n, int m, int on_threads = 0);

char **allocMatrix(int n, int m, int on_threads = 0);

#endif //PA_IOHELPERS_H
