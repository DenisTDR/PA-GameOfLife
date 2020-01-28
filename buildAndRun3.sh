#!/bin/bash

rm -rf ./gol.exe

mpicc gol.c -fopenmp -o gol.exe || exit
./gol.exe 2 0 1009 1010 4 ./inputs/02-increased.in
#./gol.exe 2 0 9 10 150 ./inputs/02-simple.in