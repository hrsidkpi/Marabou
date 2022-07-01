#!/bin/bash

mkdir -p build
cd build
module load gurobi

echo "building..."
cmake --build . -j 4
./Marabou ../resources/nnet/acasxu/ACASXU_experimental_v2a_1_1.nnet ../resources/properties/acas_property_3.txt --tightening-strategy zonotope
# ./Marabou ../resources/nnet/acasxu/ACASXU_experimental_v2a_1_1.nnet ../resources/properties/acas_property_always_sat.txt 

