#!/bin/bash

mkdir -p build
cd build

echo "building..."
cmake --build . -j 4
./Marabou ../resources/nnet/acasxu/ACASXU_experimental_v2a_1_1.nnet 