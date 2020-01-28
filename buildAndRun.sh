rm -rf exe
echo "compiling ..."
g++ -o exe main.cpp helpers/ioHelpers.cpp helpers/golSerialHelpers.cpp helpers/golHelpers.cpp || exit

echo "running ..."
./exe ./inputs/01.in

#rm exe
