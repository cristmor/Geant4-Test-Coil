#include "Data.hh"

Data::Data(const std::string& fileName):
    fFileName(fileName) {
    fFile.open(fFileName);
}

Data::~Data() {
    fFile.close();
}

// Public
std::ofstream& Data::getFile() {
    return fFile;
}
