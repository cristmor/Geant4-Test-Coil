#pragma once

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpticalPhoton.hh"
#include "G4Cerenkov.hh"
#include "Randomize.hh"

class G4ParticleGun;
class G4Event;

class Generator : public G4VUserPrimaryGeneratorAction {
public:
    Generator();    
    virtual ~Generator();
    virtual void GeneratePrimaries(G4Event* event);

private:
    G4ParticleGun*  fParticleGun;
};
