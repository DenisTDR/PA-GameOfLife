#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <unistd.h>

#define MASTER  0

#define TAG1 1
#define TAG2 2

using namespace std;

void slaveTask(int rank) {
    int nr;
    MPI_Status status;
    MPI_Recv(&nr, 1, MPI_INT, MASTER, TAG1, MPI_COMM_WORLD, &status);
    cout << "[" << rank << "] " << " received " << nr << endl;
    usleep(1000 * 1000);
}

void masterTask(int ntasks) {
//    MPI_Status status;
    for (auto i = 0; i < ntasks; i++) {
        int nr = i * 10;
        cout << "[" << 0 << "] " << "sending " << nr << " to task " << i << endl;
        MPI_Send(&nr, 1, MPI_INT, i, TAG1, MPI_COMM_WORLD);
        cout << "[" << 0 << "] " << "sent " << nr << " to task " << i << endl;
    }
    slaveTask(0);
}


int main(int argc, char *argv[]) {
    int ntasks,               /* total number of tasks in partitiion */
            rank;                 /* task identifier */
//            n,                    /* loop variable */
//            pc,                   /* prime counter */
//            pcsum,                /* number of mpi_examples found by all tasks */
//            foundone,             /* most recent prime found */
//            maxprime,             /* largest prime found */
//            mystart,              /* where to start calculating */
//            stride;               /* calculate every nth number */
//

    int n = 10;
    int m = 10;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

    if (rank == MASTER) {
        cout << "task number: " << ntasks << endl;

        masterTask(ntasks);
    } else {
        cout << "task #" << rank << " started" << endl;
        slaveTask(rank);
    }

    MPI_Finalize();
    return 0;
}
