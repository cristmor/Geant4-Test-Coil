#pragma once

#include <iostream>
#include <string>
#include <fstream>

struct Parameters {
  std::string particleName;
  size_t events;
};

class ConfigLoader {
public:
  ConfigLoader();
  ConfigLoader(const std::string& fileName);
  ~ConfigLoader();

  const Parameters& getParameters() const;

private:
  void loadFromFile();

  std::ifstream fMacfile;
  Parameters fParameter;
};
