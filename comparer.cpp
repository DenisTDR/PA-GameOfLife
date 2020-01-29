

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;


int main(int argc, char *argv[]) {

    ifstream if1, if2;
    if1.open("./inputs/mpi.out");
    if2.open("./inputs/serial.out");

    if (if1.fail() || if2.fail()) {
        cout << "failed to pen files " << endl;
        exit(-1);
    }

    int c = 0;

    string line1;
    string line2;
    while (getline(if1, line1)) {
        getline(if2, line2);
        if (line1 != line2) {
            cout << c << endl;
        }
        c++;
    }

    cout << endl << "done" << endl;


}