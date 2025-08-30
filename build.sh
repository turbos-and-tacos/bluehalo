#!/bin/bash
mkdir -p build
cd build

cmake ../third_party/googletest
make

# Compile main
g++ -std=c++20 -I../include -c ../src/movement_simulator.cpp -o movement_simulator.o
g++ -std=c++20 -I../include -c ../src/main.cpp -o main.o
g++ main.o movement_simulator.o \
    -o main

# Compile tests
echo "Compiling tests..."
g++ -std=c++20 -I../include \
    -I../third_party/googletest/googletest/include \
    -c ../tests/test_movement_simulator.cpp -o test_movement_simulator.o

g++ test_movement_simulator.o movement_simulator.o \
    ../build/lib/libgtest.a \
    ../build/lib/libgtest_main.a \
    -pthread -o test_runner


echo "Build complete."
echo "Run './build/main' for main program or './build/test_runner' for tests."
