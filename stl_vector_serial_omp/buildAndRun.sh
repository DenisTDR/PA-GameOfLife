#!/bin/bash

rm -rf ./exe
echo "compiling ..."
g++ -o exe -O3 -Wall -lgomp -fopenmp main.cpp helpers/ioHelpers.cpp helpers/golSerialHelpers.cpp helpers/golHelpers.cpp helpers/golOpenMpHelpers.cpp || exit

echo "running ..."
./exe 200 5 ../inputs/05.in

rm exe
