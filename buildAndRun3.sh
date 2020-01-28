#!/bin/bash

rm -rf ./exe

mpiCC -o exe main2.cpp || exit

mpirun -np 4 ./exe

rm ./exe