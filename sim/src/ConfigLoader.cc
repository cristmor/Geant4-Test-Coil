#include "ConfigLoader.hh"

ConfigLoader::ConfigLoader(const std::string& fileName) {
    fMacfile.open(fileName);
}

ConfigLoader::~ConfigLoader() {
    fMacfile.close();
}

// public
const Parameters ConfigLoader::getParameters() const {
    return fParameter;
}

// private
void ConfigLoader::loadFromFile() {

    while(fMacfile.good()) {
        std::string str;
        fMacfile >> str;

        if(str == "particleName") {
            fMacfile >> fParameter.particleName;
        }
        if(str == "events") {
            fMacfile >> fParameter.events;
        }
    }
}
