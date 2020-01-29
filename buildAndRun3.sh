#!/bin/bash

rm -rf ./exe

mpiCC -o exe main2.cpp helpers/ioHelpers.cpp || exit

mpirun -np 4 ./exe 200 5 ./inputs/02.in

rm ./exe