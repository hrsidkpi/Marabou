#!/bin/bash

mkdir -p build
cd build

echo "building..."
cmake --build . -j 4
module load gurobi
./Marabou ../resources/nnet/acasxu/ACASXU_experimental_v2a_1_1.nnet ../resources/properties/acas_property_3.txt 
# ./Marabou ../resources/nnet/acasxu/ACASXU_experimental_v2a_1_1.nnet ../resources/properties/acas_property_always_sat.txt 

