cmake_minimum_required(VERSION 3.15)
project(PA)

set(CMAKE_CXX_STANDARD 14)

add_executable(PA main.cpp main2.cpp mpi_examples/main.cpp mpi_examples/mpi_array.c helpers/golSerialHelpers.cpp helpers/golSerialHelpers.h helpers/ioHelpers.cpp helpers/ioHelpers.h helpers/golHelpers.cpp helpers/golHelpers.h helpers/golOpenMpHelpers.cpp helpers/golOpenMpHelpers.h)