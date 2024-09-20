#pragma once

#include "G4UserSteppingAction.hh"

#include <iostream>
#include <vector>

#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4Track.hh"
#include "Event.hh"
#include "Detector.hh"
#include "G4OpticalPhoton.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4Threading.hh"

#include "G4Electron.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"

// Custom Headers
#include "Data.hh"

class Detector;
class Event;

class Stepping : public G4UserSteppingAction {
public:
    Stepping(const Detector* detectorConstruction, Event* eventAction);
    virtual ~Stepping();
    virtual void UserSteppingAction(const G4Step* step);

private:
    const Detector* fDetConstruction;
    Event*  fEventAction;
    Data* fData;
};
