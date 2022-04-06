#!/bin/bash

mkdir -p build
cd build
module load gurobi

echo "building..."
cmake --build . -j 4