#pragma once

#include "G4VUserActionInitialization.hh"
#include "Generator.hh"
#include "Event.hh"
#include "Stepping.hh"
#include "Detector.hh"

class Detector;

class Initialization : public G4VUserActionInitialization {
public:
    Initialization(Detector*);
    virtual ~Initialization();
    virtual void Build() const;

private:
    Detector* fDetConstruction;
};
    
