#!/bin/bash
#SBATCH -J G4CoilFast
#SBATCH -o %G4CoilTest.o%j
#SBATCH -e %G4CoilTest.e%j
#SBATCH -p nocona
#SBATCH -N 1
#SBATCH --ntasks-per-node=34

# . ../../program/spack/share/spack/setup-env.sh
# spack load root
# spack load geant4

. build.sh

dir="fast"
mkdir "$dir"

cp models/*.stl "$dir"
cp "scripts/init_vis.mac" "$dir"
cp "scripts/gui.mac" "$dir"
cp "scripts/vis.mac" "$dir"
cp "scripts/$dir.mac" "$dir"
cp build/G4Coil "$dir"
cd "$dir"
mkdir data

./G4Coil -m "$dir.mac" > ../cout.txt
cp test.txt ..

cd ..
rm -rf "$dir"
