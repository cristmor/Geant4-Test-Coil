#!/bin/bash

cp ../data/temp/*.root ../data/sim/

rm -rf build
mkdir build

cd build
cmake ..
make -j32