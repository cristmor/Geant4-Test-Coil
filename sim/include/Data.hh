#pragma once

#include <iostream>
#include <string>
#include <fstream>

class Data {
public:
  Data();
  Data(const std::string& fileName);
  ~Data();

  std::ofstream& getFile();

private:
  std::ofstream fFile;
  std::string fFileName;

};
