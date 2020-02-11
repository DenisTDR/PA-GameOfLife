#!/bin/bash

rm -rf ./exe2

mpiCC -O3 -o exe2 -Wall -lgomp -fopenmp main2.cpp helpers/ioHelpers.cpp helpers/golSerialHelpers.cpp helpers/golHelpers.cpp helpers/golOpenMpHelpers.cpp || exit

mpirun -np $2 ./exe2 $1 4 ./inputs/07.in

#rm ./exe2