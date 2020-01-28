#include <vector>
#include <omp.h>
#include "helpers/ioHelpers.h"
#include "helpers/golSerialHelpers.h"
#include "helpers/golHelpers.h"
#include "helpers/golOpenMpHelpers.h"


using namespace std;

#define SERIAL 0
#define OPENMP_2TH 2
#define OPENMP_3TH 3
#define OPENMP_4TH 4
#define OPENMP_5TH 5
#define OPENMP_6TH 6
#define OPENMP_7TH 7
#define OPENMP_8TH 8

int typesToRun[] = {SERIAL, OPENMP_2TH, OPENMP_3TH, OPENMP_4TH, OPENMP_5TH, OPENMP_6TH, OPENMP_7TH, OPENMP_8TH};
//int typesToRun[] = {SERIAL, OPENMP_2TH};
int typesToRunCount = sizeof(typesToRun) / sizeof(*typesToRun);

void runAs(vector<vector<bool>> &w, int steps, int type, string &fileName) {

    for (int i = 0; i < steps; i++) {
        if (type == SERIAL) {
            gameOfLifeStepSerial(w);
        } else if (type >= OPENMP_2TH && type <= OPENMP_8TH) {
            gameOfLifeStepOpenMp(w, type);
        }
    }
}

string getRunTypeName(int type);

void writeBigMatrix(vector<vector<bool>> &w, string &fileName);

int main(int argc, char **argv) {

    if (argc != 4) {
        printf("Usage: %s [Steps] [TestCount] <filename>\n", argv[0]);
        return 0;
    }

    int steps_count = stoi(argv[1]);
    int test_count = stoi(argv[2]);
    string fileName = argv[3];

    vector<vector<bool>> initialWorldConfig, world;


    cout << "fileName: " << fileName << endl;
    loadFromFile(initialWorldConfig, fileName);
    addPadding(initialWorldConfig, 1, 1, 1, 1);

    cout << "size: " << initialWorldConfig.size() - 2 << "x"
         << (!initialWorldConfig.empty() ? initialWorldConfig[0].size() - 2 : -1) << endl;

    cout << "tests per type: " << test_count << endl;
    cout << "steps: " << steps_count << endl;

    double avgTimes[typesToRunCount];


    int lastAliveCellsCount = -1;

    for (int t = 0; t < typesToRunCount; t++) {
        auto runType = typesToRun[t];
        double timeForType = 0;
        cout << "\n" << getRunTypeName(runType) << endl;
        for (int i = 0; i < test_count; i++) {
            double tbegin = omp_get_wtime();

            world = initialWorldConfig;
            runAs(world, steps_count, runType, fileName);


            double time_spent = omp_get_wtime() - tbegin;
            timeForType += time_spent;
            cout << "     #" << i << ": " << time_spent << "s" << endl;

            auto crtAliveCells = countAliveCells(world);
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

    cout << endl;
    cout << "Alive cells " << lastAliveCellsCount << endl;
    cout << "steps: " << steps_count << endl;
    cout << "size: " << initialWorldConfig.size() - 2 << "x"
         << (!initialWorldConfig.empty() ? initialWorldConfig[0].size() - 2 : -1) << endl;
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


void writeBigMatrix(vector<vector<bool>> &w, string &fileName) {
    ofstream out;
    out.open(fileName);
    if (out.fail()) {
        cout << "can't open file: " << fileName << endl;
        exit(-1);
    }

    out << w.size() * 10 << " " << w[0].size() * 10 << "\n";

    for (int j = 0; j < 10; j++)
        for (const auto &v : w) {
            for (int i = 0; i < 10; i++) {
                for (auto x : v) {
                    out << x << " ";
                }
            }
            out << "\n";
        }
    out.flush();
    out.close();
}


//            string x = "./inputs/02-after-" + to_string(steps_count) + "-steps.in";
//            string x = "./inputs/05-steps.in";
//            writeBigMatrix(world, x);
//            exit(-1);