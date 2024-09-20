#include "Stepping.hh"

Stepping::Stepping(const Detector* detectorConstruction, Event* eventAction)
    : G4UserSteppingAction(), fDetConstruction(detectorConstruction), fEventAction(eventAction) {
    fData = new Data("test.txt");
}
Stepping::~Stepping() {
    delete fData;
}

// Public
void Stepping::UserSteppingAction(const G4Step* step) {
    auto track = step ->GetTrack();
    static G4ParticleDefinition* opticalphoton = G4OpticalPhoton::OpticalPhotonDefinition();
    const G4ParticleDefinition* particleDef = track->GetDynamicParticle()->GetParticleDefinition();
    G4String thisName = step->GetPreStepPoint()->GetTouchable()->GetVolume()->GetName();

    if(particleDef == opticalphoton)
    {
        G4cout
            <<" particleName=" << step->GetTrack()->GetDefinition()->GetParticleName()
            <<" ," << thisName
            <<" ,Positison=" << step->GetTrack()->GetPosition()
            << G4endl;

        // write to file
        fData->getFile() 
            <<" particleName=" << step->GetTrack()->GetDefinition()->GetParticleName()
            <<" ," << thisName
            <<" ,Positison=" << step->GetTrack()->GetPosition()
            << std::endl;
    }
}

// Private
