#include "mpi.h"
#include "helpers/ioHelpers.h"
#include "helpers/golSerialHelpers.h"
#include "helpers/golHelpers.h"
#include "helpers/golOpenMpHelpers.h"
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <omp.h>

#define MASTER  0

//#define TAG1 1
//#define TAG2 2

#define RUNNER_CONFIG 3
#define SLICE_ROW 4
#define ROW_UP_TO_BOTTOM 6
#define ROW_BOTTOM_TO_UP 7

#define THREADS 1


typedef struct RunnerConfig {
    int rank;
    int cols;
    int slice_rows;
    int offset;
    int steps_count;
    int test_count;


    RunnerConfig() {}

    RunnerConfig(int _cols, int _slice_rows, int _offset, int _steps_count, int _test_count) :
            cols(_cols), slice_rows(_slice_rows), offset(_offset), steps_count(_steps_count), test_count(_test_count) {
    }
} RunnerConfig;

using namespace std;

void someDebug(RunnerConfig &rc, int step, int rowsAvailable, int recUp, int offsetTop) {
    std::stringstream ss;
    ss << "[" << rc.rank << "]" << " step " << step << " processing " << rowsAvailable
       << " lines (";
    // 2  2
    for (int i = 1 + !recUp; i < offsetTop; i++) {
        ss << i + rc.offset << ", ";
    }
    ss << ")" << endl;
    cout << ss.str();
}

void slaveTask(int rank, int ntasks, bool **&someMatrix) {
    RunnerConfig rc;
    MPI_Status status;
    MPI_Recv(&rc, sizeof(RunnerConfig), MPI_BYTE, MASTER, RUNNER_CONFIG, MPI_COMM_WORLD, &status);
    rc.rank = rank;
//    cout << "[" << rc.rank << "] " << " received slice_size=" << rc.slice_rows << " offset=" << rc.offset << endl;

    bool **matrix;

    if (rank != 0) {
        matrix = allocMatrix(rc.slice_rows + 2, rc.cols + 2);
        for (auto j = 0; j < rc.slice_rows + 2; j++) {
            MPI_Recv(matrix[j], rc.cols, MPI_BYTE, MASTER, SLICE_ROW, MPI_COMM_WORLD, &status);
        }
//        cout << "[" << rc.rank << "] " << " received slices" << endl;
    } else {
        matrix = cloneMatrix(someMatrix, rc.slice_rows + 2, rc.cols);
//        cout << "[" << rc.rank << "] " << "cloned matrix" << endl;
    }

//    displayMatrix(matrix, rc.slice_rows, rc.cols, rc.rank + 100, false);
//    usleep(500 * 1000);


    bool **tmpMatrix = allocMatrix(rc.slice_rows + 2, rc.cols + 2);

    /*
//    std::stringstream ss;
//    ss << "[" << rc.rank << "]\n";
//    for (auto j = 0; j < rc.slice_rows + 2; j++) {
//        for (auto k = 0; k < rc.cols + 2; k++) {
//            ss << matrix[j][k] << " ";
//        }
//        ss << "\n";
//    }
//    ss << "\n";
//
//    cout << ss.str();
     */
    MPI_Request request[2], send_req[2];
    MPI_Status recv_status[2];


    int recUp, recBottom;
    for (auto step = 0; step < rc.steps_count; step++) {
//        cout << "[" << rc.rank << "] start step " << step << endl;
//        usleep(50 * 1000 * (rc.rank + 1));
        recUp = recBottom = true;
        if (step != 0) {
            if (rc.rank != 0) {
                recUp = false;
                MPI_Irecv(matrix[0], rc.cols, MPI_BYTE, rc.rank - 1, ROW_BOTTOM_TO_UP, MPI_COMM_WORLD, &request[0]);
            }
            if (rc.rank != ntasks - 1) {
                recBottom = false;
                MPI_Irecv(matrix[rc.slice_rows + 1], rc.cols, MPI_BYTE, rc.rank + 1, ROW_UP_TO_BOTTOM, MPI_COMM_WORLD,
                          &request[1]);
            }
            if (rc.rank != 0) {
                MPI_Isend(matrix[1], rc.cols, MPI_BYTE, rc.rank - 1, ROW_UP_TO_BOTTOM, MPI_COMM_WORLD, &send_req[0]);
            }
            if (rc.rank != ntasks - 1) {
                MPI_Isend(matrix[rc.slice_rows], rc.cols, MPI_BYTE, rc.rank + 1, ROW_BOTTOM_TO_UP, MPI_COMM_WORLD,
                          &send_req[0]);
            }
        }

//        cout << "[" << rc.rank << "] " << " before while" << endl;

//        cout << "[" << rc.rank << "] " << " after while" << endl;
        int rowsAvailable = rc.slice_rows - !recUp - !recBottom;
        int offsetTop = 1 + !recUp + rowsAvailable;
//        someDebug(rc, step, rowsAvailable, recUp, offsetTop);
        gameOfLifeOpenMp2(matrix, tmpMatrix, offsetTop, rc.cols, !recUp, true, rc.offset,
                          "[" + to_string(rc.rank) + "]", THREADS);



//        displayMatrix(matrix, rc.slice_rows, rc.cols, -1, false);

        while (!recUp || !recBottom) {
            if (!recUp) {
                MPI_Test(&request[0], &recUp, &recv_status[0]);
                if (recUp) {
//                    cout << "[" << rc.rank << "] " << " received up (" << rc.offset << ")" << endl;
                    gameOfLifeOpenMp2(matrix, tmpMatrix, 2, rc.cols, 0,
                                      true,
                                      rc.offset,
                                      "[" + to_string(rc.rank) + "]", THREADS);
                }
            }
            if (!recBottom) {
                MPI_Test(&request[1], &recBottom, &recv_status[1]);
                if (recBottom) {
//                    cout << "[" << rc.rank << "] " << " received bottom (" << rc.offset + rc.slice_rows << ")" << endl;
                    gameOfLifeOpenMp2(matrix, tmpMatrix, rc.slice_rows + 1, rc.cols, rc.slice_rows - 1,
                                      true,
                                      rc.offset,
                                      "[" + to_string(rc.rank) + "]", THREADS);
                }
            }
        }


        swap(matrix, tmpMatrix);

//        usleep(500 * 1000);
//        cout << "----" << endl;

    }

//    cout << "[" << rc.rank << "] " << " processing done, sending data to master" << endl;
    MPI_Barrier(MPI_COMM_WORLD);


    // send slices to master
    if (rank != 0) {
        for (auto j = 1; j < rc.slice_rows + 1; j++) {
            MPI_Send(matrix[j], rc.cols, MPI_BYTE, MASTER, SLICE_ROW, MPI_COMM_WORLD);
        }
    } else {
        copyMatrixContent(matrix, someMatrix, rc.slice_rows, rc.cols, 0);
    }
//    cout << "[" << rc.rank << "] " << " sent data to master" << endl;

//    usleep(1000 * 1000);
}

void masterTask(int ntasks, bool **&matrix, int n, int m, int steps_count, int test_count) {

    int slice_size = n / ntasks;

    int master_slice_size = slice_size + (n % ntasks);
    RunnerConfig rcs[ntasks];

    int offset = 0;

//    cout << "[0] sending configs" << endl;
    //send runner config to each slave
    for (auto i = 0; i < ntasks; i++) {
        int sz = i == 0 ? master_slice_size : slice_size;
        rcs[i] = RunnerConfig(m, sz, offset, steps_count, test_count);
        MPI_Send(&(rcs[i]), sizeof(RunnerConfig), MPI_BYTE, i, RUNNER_CONFIG, MPI_COMM_WORLD);
        offset += sz;
    }

//    displayMatrix(matrix, n, m, -1, false);
//    cout << "[0] sending slices" << endl;
    //send matrix slice to each slave
    for (auto i = 1; i < ntasks; i++) {
        RunnerConfig rc = rcs[i];
//        cout << "sending slices to " << i << endl;
        for (auto j = 0; j < rc.slice_rows + 2; j++) {
//            cout << "sending slice " << j << " to " << i << endl;
            MPI_Send(matrix[j + rc.offset], rc.cols, MPI_BYTE, i, SLICE_ROW, MPI_COMM_WORLD);
        }
    }

    slaveTask(0, ntasks, matrix);

//    displayMatrix(matrix, n, m, -1, false);
    MPI_Status status;
    // receive matrix slices
    for (auto i = 1; i < ntasks; i++) {
        RunnerConfig rc = rcs[i];
        for (auto j = 1; j < rc.slice_rows + 1; j++) {
            MPI_Recv(matrix[j + rc.offset], rc.cols, MPI_BYTE, i, SLICE_ROW, MPI_COMM_WORLD, &status);
        }
    }
//    displayMatrix(matrix, n, m, -1, false);
//
//    writeBigMatrix(matrix, n, m, "./inputs/mpi.out");
    auto aliveCells = countAliveCells(matrix, n, m);
    cout << "alive cells: " << aliveCells << endl;
}


int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s [Steps] [TestCount] <filename>\n", argv[0]);
        return -1;
    }
    int steps_count = stoi(argv[1]);
    int test_count = stoi(argv[2]);
    string fileName = argv[3];

    bool **matrix;
    int n, m;

    int ntasks, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

    if (rank == MASTER) {
        cout << "task number: " << ntasks << endl;

        double tbegin = omp_get_wtime();
        doInitialWork(matrix, n, m, steps_count, test_count, fileName);
        masterTask(ntasks, matrix, n, m, steps_count, test_count);


        double time_spent = omp_get_wtime() - tbegin;
        cout << "MPI    - " << ntasks << " tasks      " << time_spent << "s" << endl;
    } else {
//        cout << "task #" << rank << " started" << endl;
        slaveTask(rank, ntasks, matrix);
    }


    MPI_Finalize();
    return 0;
}

