rm -rf exe
echo "compiling ..."
g++ -o exe -O3 -Wall -lgomp -fopenmp main.cpp helpers/ioHelpers.cpp helpers/golSerialHelpers.cpp helpers/golHelpers.cpp helpers/golOpenMpHelpers.cpp || exit

echo "running ..."
./exe ./inputs/02.in

#rm exe
