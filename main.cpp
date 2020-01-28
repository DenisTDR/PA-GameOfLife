#include <vector>
#include <unistd.h>
#include "helpers/ioHelpers.h"
#include "helpers/golSerialHelpers.h"
#include "helpers/golHelpers.h"

using namespace std;

int main(int argc, char **argv) {
    vector<vector<bool>> world;
    auto steps = 50;

    cout << argc << endl;

    loadFromFile(world, argc == 1 ? "./inputs/02.in": argv[1]);

    addPadding(world, 10, 10, 10, 10);


    for (int i = 0; i < steps; i++) {
        displayMatrix(world, i);
        usleep(200 * 1000);
        gameOfLifeStep(world);
    }

    return 0;
}

