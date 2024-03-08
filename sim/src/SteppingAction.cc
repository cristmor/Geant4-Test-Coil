//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// 
/// \file SteppingAction.cc
/// \brief Implementation of the SteppingAction class

#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
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

#include <iostream>
#include <vector>

// #include "TH1D.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(
                      const DetectorConstruction* detectorConstruction,
                      EventAction* eventAction,
                      CoilTree* coilTree)
  : G4UserSteppingAction(),
    fDetConstruction(detectorConstruction),
    fEventAction(eventAction),
    fCoilTree(coilTree)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{

  auto track = step ->GetTrack();
  static G4ParticleDefinition* opticalphoton = G4OpticalPhoton::OpticalPhotonDefinition();
  const G4ParticleDefinition* particleDef = track->GetDynamicParticle()->GetParticleDefinition();
  G4String thisName = step->GetPreStepPoint()->GetTouchable()->GetVolume()->GetName();
  const G4Event* currentEvent = G4EventManager::GetEventManager()->GetConstCurrentEvent();

  G4int eventNumber = currentEvent->GetEventID();


  if(particleDef == opticalphoton)
  {
    PrintStep(step);
    // if((thisName == "FiberCorePV" or thisName == "G4CoilCorePV") && track->GetPosition().z() > 490) track->SetTrackStatus(fStopAndKill);
    // fCoilTree->GetThreadData(G4Threading::G4GetThreadId())->AddToTree(step, eventNumber);
  }
}

// ============================================================================

void SteppingAction::PrintStep(const G4Step* step){
  auto secondary = step->GetTrack();
  G4int TrackID = secondary->GetTrackID();
  G4int steps = secondary ->GetCurrentStepNumber();

  const G4DynamicParticle* dynamicParticle= secondary ->GetDynamicParticle();
  G4ParticleDefinition* particle = dynamicParticle->GetDefinition();
  G4String particleName = particle->GetParticleName();

  auto touchable = step->GetPreStepPoint()->GetTouchable();
  auto thisPhysical = touchable->GetVolume(); // mother
  auto thisCopyNo = thisPhysical->GetCopyNo();
  auto thisName = thisPhysical->GetName();

  G4String CreationProcess = secondary->GetCreatorProcess()->GetProcessName();
  G4ThreeVector Positison = secondary->GetPosition();
  
  G4cout<<" tk=" << TrackID
        <<" ,steps=" << steps
        <<" ,particleName=" << particleName
        <<" ," << thisName
        <<" ,Copy No:" << thisCopyNo 
        <<" ,CreationProcess=" << CreationProcess
        <<" ,Positison=" << Positison
        << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
