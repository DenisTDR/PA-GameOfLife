#!/bin/bash

rm -rf ./exe

mpiCC -o exe main.cpp || exit

#srun -N2 -n8 -ppReserved exe

mpirun -np 4 ./exe

rm ./exe