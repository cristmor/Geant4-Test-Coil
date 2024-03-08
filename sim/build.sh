# . ../../program/spack/share/spack/setup-env.sh
# spack load root
# spack load geant4

rm -rf build
mkdir build

cd build
cmake ..
make -j32
cd ..

