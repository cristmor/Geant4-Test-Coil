#!/bin/bash

cp ../data/temp/*.root ../data/sim/

rm -rf build
mkdir build

cd build
cmake ..
make -j32

specific_directory="../../data/sim"  # Replace with the desired directory path
ana_directory="../../data/ana"       # Replace with the directory path where analyzed files are stored
newest_file=$(ls -t "$specific_directory" | head -n 1)

if [ -z "$newest_file" ]; then
  echo "No files found in the directory: $specific_directory"
  exit 1
fi

# Loop through all files in specific_directory
for file in "$specific_directory"/*.root; do
  filename=$(basename "$file")
  ana_file="$ana_directory/ana_$filename"
  echo "Checking $file"

  # Check if the file has not been analyzed (ana_file does not exist)
  if [ ! -f "$ana_file" ]; then
    # Use the current file as a parameter in the script
    echo "Analyzing $filename"
    ./Ana "$filename"
  fi
done

cd ..
