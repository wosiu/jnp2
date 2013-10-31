#!/bin/bash

make

for i in tes*.cc
do
    echo "Wykonuje test - $i";
    gcc -std=c++0x -c $i -o tmp.o || { echo "ERROR 1"; exit 1; }
    g++ tmp.o network.o growingnet.o -o spr || { echo "ERROR 2"; exit 2; }
    time ./spr || { echo "ERROR 3"; exit 3; }
    g++ tmp.o growingnet.o network.o -o spr || { echo "ERROR 4"; exit 4; }
    ./spr || { echo "ERROR 5"; exit 5; }
    rm tmp.o spr || { echo "ERROR 6"; exit 6; }
done
