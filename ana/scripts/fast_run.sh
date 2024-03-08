#!/bin/bash

. scripts/build.sh

./Ana 10GeV_pi+_100000_2023-08-07_16-49-26.root > cout.txt

cd ..

root -l ../data/ana/ana_10GeV_pi+_100000_2023-08-07_16-49-26.root