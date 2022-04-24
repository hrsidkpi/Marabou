#!/bin/bash

read -p "Continue (y/n)?" CONT
if [ "$CONT" = "y" ]; then
    echo "deleting current installation..." 
    rm -r build
    rm CMakeCache.txt
    mkdir build
    cd build 
    cmake .. -DFETCHCONTENT_QUIET=OFF 
    echo "building..."
    cmake --build . -j 4
else
    echo "booo";
fi
