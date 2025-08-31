#!/bin/bash

set -e

BUILD_GTEST=true
for arg in "$@"; do
    if [[ "$arg" == "--no-gtest" ]]; then
        BUILD_GTEST=false
    fi
done

mkdir -p build
cd build

if $BUILD_GTEST; then 
    cmake ../third_party/googletest
    make
fi

# Compile main
g++ -std=c++20 -I../include -c ../src/movement_simulator.cpp -o movement_simulator.o
g++ -std=c++20 -I../include -c ../src/main.cpp -o main.o
g++ main.o movement_simulator.o \
    -o main

# Compile tests
if $BUILD_GTEST; then 
    echo "Compiling tests..."
    g++ -std=c++20 -I../include \
        -I../third_party/googletest/googletest/include \
        -c ../tests/test_movement_simulator.cpp -o test_movement_simulator.o

    g++ test_movement_simulator.o movement_simulator.o \
        ../build/lib/libgtest.a \
        ../build/lib/libgtest_main.a \
        -pthread -o test_runner
fi

echo "Build complete."
echo "Run './build/main' for main program"
if $BUILD_GTEST; then 
    echo "Run'./build/test_runner' for unit tests."
fi
