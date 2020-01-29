#include "mpi.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <tgmath.h>

using namespace std;

int main(int argc, char *argv[]) {
    int size, rank, tag, rc, N, generations, outPoints, s, g, slice_height, slice_width, r, c;

    MPI_Status Stat;
    int **theBoard;
    ofstream output("output_mpi_2D.txt");    //output file
    rc = MPI_Init(&argc, &argv);
    // if (rc!=0) {cout << "Error starting MPI." << endl; MPI_Abort(MPI_COMM_WORLD, rc);}
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        // I am processor 0
        if (argc < 2) {
            // cout << "Input file not specified" << endl;
            MPI_Finalize();
            return -1;
        }

        ifstream file(argv[1]);    //input file

        if (!file) {
            // cout << "Error opening file"<<endl;
            MPI_Finalize();
            return -1;
        }

        file >> N >> generations;    //first three variables from file
        theBoard = (int **) calloc(N, sizeof(int *));
        for (int i = 0; i < N; i++) {
            theBoard[i] = (int *) calloc(N, sizeof(int));
        }

        // cout << N << " " << generations << " " << outPoints << endl;
        while (!file.eof()) {
            int x, y;
            char space;
            file >> x >> space >> y;
            // cout << x << " " << y << endl;
            theBoard[x][y] = 1;
        }

        file.close();

        r = int(sqrt(size));
        c = int(size / r);
        slice_height = N / r + 2;
        slice_width = N / c + 2;
        int localinfo[2];

        localinfo[0] = slice_height;
        localinfo[1] = slice_width;
        string start = "start";
        for (int s = 0; s < size; s++) {
            MPI_Send(&generations, 1, MPI_INT, s, 1, MPI_COMM_WORLD);    //and send it
            MPI_Send(&localinfo, 2, MPI_INT, s, 1, MPI_COMM_WORLD);    //and send it
        }


        // for (int i = 0; i < 10; i++)
        // {
        // for (int j = 0; j < 10; j++)
        // {
        // cout << theBoard[i][j] << " ";
        // }
        // cout << endl;
        // }

        // s=N/(size/2);	//how many slices

    }
    MPI_Recv(&generations, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &Stat);
    //RECEIVED INITIAL INFORMATION
    int localinfo[2];        // local info for initial information
    MPI_Recv(&localinfo, 2, MPI_INT, 0, 1, MPI_COMM_WORLD, &Stat);    //receive info
    // cout << rank << " "<< localinfo[0] << endl;
    // cout << rank << " "<< localinfo[1] << endl;

    int sl_height = localinfo[0];            //
    int sl_width = localinfo[1];            //

    for (int g = 0; g < generations; g++) //generations forloop
    {
        if (rank == 0) {
            // cout << "blaasas" <<endl;

            int slice[sl_height][sl_width];
            // cout << "blaasas" <<endl;
            for (int z = 0; z < r; z++) {
                for (int k = 0; k < c; k++) {
                    for (int l = 0; l < sl_height; l++) {
                        for (int m = 0; m < sl_width; m++) {
                            if ((z == 0 && l == 0) || (z == r - 1 && l == (sl_height - 1)) || (k == 0 && m == 0) ||
                                (k == c - 1 && m == (sl_width - 1))) {
                                slice[l][m] = 0;
                            } else {
                                slice[l][m] = theBoard[l - 1 + (z * (sl_height - 2))][m - 1 + k * (sl_width -
                                                                                                   2)];    //cut a slice from the the board
                            }
                        }
                    }
                    // for (int i = 0; i < sl_height; i++)
                    // {
                    // 	for (int j = 0; j < sl_width; j++)
                    // 	{
                    // cout <<  slice[i][j] << " ";
                    // 	}
                    // cout  << "r" << rank << " " <<endl;
                    // }
                    if (z != 0 || k != 0) {
                        MPI_Send(&slice, sl_height * sl_width, MPI_INT, z * c + k, 1, MPI_COMM_WORLD);    //and send it

                    }
                    // cout <<"sent to: " <<  z*c+k <<" " << z <<" "<< k<< endl;
                }
            }

        } // end of processor 0 code
        // cout << "bla"<<endl;
        int myslice[sl_height][sl_width]; //my own slice of the board
        if (rank != 0) {
            MPI_Recv(&myslice, localinfo[0] * localinfo[1], MPI_INT, 0, 1, MPI_COMM_WORLD, &Stat);    //receive slice
            // cout <<rank << " myslice" << endl;
        } else {
            // cout <<rank << " myslice1" << endl;

            int z = 0;
            int k = 0;
            for (int l = 0; l < sl_height; l++) {
                for (int m = 0; m < sl_width; m++) {
                    if ((z == 0 && l == 0) || (z == r - 1 && l == (sl_height - 1)) || (k == 0 && m == 0) ||
                        (k == c - 1 && m == (sl_width - 1))) {
                        myslice[l][m] = 0;
                    } else {
                        myslice[l][m] = theBoard[l - 1 + (z * (sl_height - 2))][m - 1 + k * (sl_width -
                                                                                             2)];    //cut a slice from the the board
                    }
                }
            }
        }
        // cout << "bla1"<<endl;
        // for (int i = 0; i < sl_height; i++)
        // {
        // 	for (int j = 0; j < sl_width; j++)
        // 	{
        // cout <<  myslice[i][j] << " ";
        // 	}
        // cout  << "r" << rank << " " << endl;
        // }



        //COUNTING NEIGHBORS
        int sum = 0; // sum of neighbours
        int mynewslice[sl_height][sl_width];
        for (int i = 0; i < sl_width; i++) {
            mynewslice[0][i] = 0;
            mynewslice[sl_height - 1][i] = 0;
        }

        for (int i = 0; i < sl_height; i++) {
            mynewslice[i][0] = 0;
            mynewslice[i][sl_width - 1] = 0;
        }

        for (int x = 1; x < sl_height - 1; x++) //for each row
        {
            for (int y = 1; y < sl_width - 1; y++) //for each column
            {
                sum = 0;
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        if (i == 0 & j == 0) {
                            continue;
                        }
                        sum += myslice[x + i][y + j];
                    }
                }
                //PUT THE NEW VALUE OF A CELL
                if (myslice[x][y] == 1 && (sum == 2 || sum == 3)) mynewslice[x][y] = 1;
                else if (myslice[x][y] == 1 && sum > 3) mynewslice[x][y] = 0;
                else if (myslice[x][y] == 1 && sum < 1) mynewslice[x][y] = 0;
                else if (myslice[x][y] == 0 && sum == 3) mynewslice[x][y] = 1;
                else mynewslice[x][y] = 0;

            }
        }
        // cout << rank << " mynewslice" << endl;
        // for (int i = 0; i < sl_height; i++)
        // {
        // 	for (int j = 0; j < sl_width; j++)
        // 	{
        // cout << mynewslice[i][j] << " ";
        // 	}
        // cout  <<  "r" << rank << " " << endl;
        // }

        if (rank != 0)
            MPI_Send(&mynewslice, sl_height * sl_width, MPI_INT, 0, 1, MPI_COMM_WORLD);

        //PRINTING THE RESULT TO FILE
        if (rank == 0) {
            int aBoard[slice_height][slice_width];
            output << "Generation " << g << ":" << endl;
            for (int x = 1; x < slice_height - 1; x++) //put your own slice
            {
                for (int y = 1; y < slice_width - 1; y++) {
                    theBoard[x - 1][y - 1] = mynewslice[x][y];
                }
            }
            for (int i = 1; i < size; i++) {
                int row_num = i / c;
                int col_num = i - row_num * c;
                // cout << i << "i " << row_num << "r " << col_num  <<"c"<< endl;
                MPI_Recv(&aBoard, slice_height * slice_width, MPI_INT, i, 1, MPI_COMM_WORLD,
                         &Stat); //receive all others'
                // cout << "A board" << endl;
                // for (int i = 0; i < slice_height; i++)
                // {
                // 	for (int j = 0; j < slice_width; j++)
                // 	{
                // cout << aBoard[i][j] << " ";
                // 	}
                // cout  << endl;
                // }
                // output << endl << endl;

                for (int x = 1; x < slice_height - 1; x++) {
                    for (int y = 1; y < slice_width - 1; y++) {
                        theBoard[row_num * (slice_height - 2) + x - 1][col_num * (slice_width - 2) + y -
                                                                       1] = aBoard[x][y];
                        // cout << "row " << row_num * (slice_height - 2) + x-1 << " col " << col_num * (slice_width-2) + y-1 << endl;
                    }
                }
            }
            // cout << "the board" << endl;
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    if (theBoard[i][j] == 1) {
                        output << i << "," << j << endl;
                    }
                    // cout << theBoard[i][j] << " ";
                }
                // cout  << endl;
            }
            // cout  << endl;
            output << endl << endl;
        }
    }
    output.close();
    if (rank == 0) {
        free(theBoard);
    }
    MPI_Finalize();
    return 0;
}
