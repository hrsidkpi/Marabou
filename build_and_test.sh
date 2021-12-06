#!/bin/bash

mkdir -p build
cd build

echo "building..."
cmake --build . -j 4