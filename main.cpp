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

#define STEP_COUNT 150
#define TEST_COUNT 5

int typesToRun[] = {SERIAL, OPENMP_2TH, OPENMP_3TH, OPENMP_4TH, OPENMP_5TH, OPENMP_6TH, OPENMP_7TH, OPENMP_8TH};
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

int main(int argc, char **argv) {

    vector<vector<bool>> initialWorldConfig, world;

    string fileName = argc == 1 ? "./inputs/01.in" : argv[1];
    cout << "fileName: " << fileName << endl;
    loadFromFile(initialWorldConfig, fileName);
    addPadding(initialWorldConfig, 500, 500, 500, 500);
    cout << "size: " << initialWorldConfig.size() << "x"
         << (!initialWorldConfig.empty() ? initialWorldConfig[0].size() : -1) << endl;
    cout << "steps: " << STEP_COUNT << endl;

    double avgTimes[typesToRunCount];


    int lastAliveCellsCount = -1;

    for (int t = 0; t < typesToRunCount; t++) {
        auto runType = typesToRun[t];
        double timeForType = 0;
        cout << "\n" << getRunTypeName(runType) << endl;
        for (int i = 0; i < TEST_COUNT; i++) {
            double tbegin = omp_get_wtime();

            world = initialWorldConfig;
            runAs(world, STEP_COUNT, runType, fileName);


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
        avgTimes[t] = timeForType / TEST_COUNT;
        cout << "    Avg: " << avgTimes[t] << "\n";
    }


    cout << endl;
    for (int t = 0; t < typesToRunCount; t++) {
        auto runType = typesToRun[t];

        cout << getRunTypeName(runType) << "    " << avgTimes[t] << "s" << endl;

    }


    return 0;
}

string getRunTypeName(int type) {

    if (type == 0) {
        return "SERIAL";
    }
    if (type >= OPENMP_2TH && type <= OPENMP_8TH) {
        return "OpenMP - " + to_string(type) + " threads";
    }

    return "invalid run type!!!";
}

