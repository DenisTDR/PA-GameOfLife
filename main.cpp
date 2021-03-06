#include <vector>
#include <omp.h>
#include "helpers/ioHelpers.h"
#include "helpers/golSerialHelpers.h"
#include "helpers/golHelpers.h"
#include "helpers/golOpenMpHelpers.h"
#include <string>

using namespace std;

#define SERIAL 0
#define OPENMP_2TH 2
#define OPENMP_3TH 3
#define OPENMP_4TH 4
#define OPENMP_5TH 5
#define OPENMP_6TH 6
#define OPENMP_7TH 7
#define OPENMP_8TH 8

//int typesToRun[] = {SERIAL, OPENMP_2TH, OPENMP_3TH, OPENMP_4TH, OPENMP_5TH, OPENMP_6TH, OPENMP_7TH, OPENMP_8TH};
int typesToRun[] = {SERIAL, OPENMP_2TH, OPENMP_4TH};
//int typesToRun[] = {SERIAL};
int typesToRunCount = sizeof(typesToRun) / sizeof(*typesToRun);
bool **someNullPtr = nullptr;
string rand123;

void runAs(bool **&w, int n, int m, int steps, int type, string &fileName) {
//    displayMatrix(w, n, m, 0, false);
    for (int i = 0; i < steps; i++) {
        if (type == SERIAL) {
            gameOfLifeStepSerial(w, someNullPtr, n, m, 0, false, 0, rand123);

//            displayMatrix(w, n, m, i + 1, false);
        } else if (type >= OPENMP_2TH && type <= OPENMP_8TH) {
            gameOfLifeStepOpenMp(w, n, m, type);
        }
    }
}

string getRunTypeName(int type);

//void writeBigMatrix(vector<vector<bool>> &w, string &fileName);

int main(int argc, char **argv) {

    if (argc != 4) {
        printf("Usage: %s [Steps] [TestCount] <filename>\n", argv[0]);
        return 0;
    }

    int steps_count = stoi(argv[1]);
    int test_count = stoi(argv[2]);
    string fileName = argv[3];

    bool **initialWorld, **tmpMatrix = nullptr;
    int n, m;

    doInitialWork(initialWorld, n, m, steps_count, test_count, fileName);

//    tmpMatrix[7][2] = 0;
//    swap(initialWorld, tmpMatrix);
//    displayMatrix(initialWorld, n, m, -1, false);
//    displayMatrix(tmpMatrix, n, m, -1, false);

    double avgTimes[typesToRunCount];


    int lastAliveCellsCount = -1;

    for (int t = 0; t < typesToRunCount; t++) {
        auto runType = typesToRun[t];
        double timeForType = 0;
        cout << "\n" << getRunTypeName(runType) << endl;
        for (int i = 0; i < test_count; i++) {
            double tbegin = omp_get_wtime();

            if (tmpMatrix == nullptr) {
                tmpMatrix = cloneMatrix(initialWorld, n, m);
            } else {
                copyMatrixContent(initialWorld, tmpMatrix, n, m);
            }

            runAs(tmpMatrix, n, m, steps_count, runType, fileName);


            double time_spent = omp_get_wtime() - tbegin;
            timeForType += time_spent;
            cout << "     #" << i << ": " << time_spent << "s" << endl;

            auto crtAliveCells = countAliveCells(tmpMatrix, n, m);
            if (lastAliveCellsCount == -1) {
                lastAliveCellsCount = crtAliveCells;
            } else if (lastAliveCellsCount != crtAliveCells) {
                cout << "found different number of alive cells between tests: " << lastAliveCellsCount << " "
                     << crtAliveCells << endl;
                exit(1);
            }
        }
        avgTimes[t] = timeForType / test_count;
        cout << "    Avg: " << avgTimes[t] << "\n";
    }

//    writeBigMatrix(tmpMatrix, n, m, "./inputs/serial.out");
//    displayMatrix(tmpMatrix, n, m, -1, false);

    cout << endl;
    cout << "Alive cells " << lastAliveCellsCount << endl;
    cout << "steps: " << steps_count << endl;
    cout << "size: " << n << "x" << m << endl;
    for (int t = 0; t < typesToRunCount; t++) {
        auto runType = typesToRun[t];
        cout << getRunTypeName(runType) << "    " << avgTimes[t] << "s" << endl;
    }

    return 0;
}

string getRunTypeName(int type) {

    if (type == 0) {
        return "SERIAL            ";
    }
    if (type >= OPENMP_2TH && type <= OPENMP_8TH) {
        return "OpenMP - " + to_string(type) + " threads";
    }

    return "invalid run type!!!";
}
//
//
//void writeBigMatrix(vector<vector<bool>> &w, string &fileName) {
//    ofstream out;
//    out.open(fileName);
//    if (out.fail()) {
//        cout << "can't open file: " << fileName << endl;
//        exit(-1);
//    }
//
//    out << w.size() * 10 << " " << w[0].size() * 10 << "\n";
//
//    for (int j = 0; j < 10; j++)
//        for (const auto &v : w) {
//            for (int i = 0; i < 10; i++) {
//                for (auto x : v) {
//                    out << x << " ";
//                }
//            }
//            out << "\n";
//        }
//    out.flush();
//    out.close();
//}


//            string x = "./inputs/02-after-" + to_string(steps_count) + "-steps.in";
//            string x = "./inputs/05-steps.in";
//            writeBigMatrix(world, x);
//            exit(-1);