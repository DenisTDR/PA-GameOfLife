#!/bin/bash

rm -rf ./exe
echo "compiling ..."
g++ -o exe -O3 -Wall -lgomp -fopenmp main.cpp helpers/ioHelpers.cpp helpers/golSerialHelpers.cpp helpers/golHelpers.cpp helpers/golOpenMpHelpers.cpp || exit

echo "running ..."
./exe $1 4 ./inputs/07.in

#rm exe
