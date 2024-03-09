#include "Generator.hh"

Generator::Generator()
    : G4VUserPrimaryGeneratorAction(), fParticleGun(nullptr){
    fParticleGun = new G4ParticleGun(1);

    auto particleDefinition = G4OpticalPhoton::OpticalPhotonDefinition();
    // auto particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("e-");
    fParticleGun->SetParticleDefinition(particleDefinition);
    fParticleGun->SetParticleEnergy(100.0*keV);

    G4double startX = 0.5*cm;
    G4double startY = -0.09*cm;
    G4double startZ = 0.50*cm;

    G4double px = 0.0;
    G4double py = 0;
    G4double pz = -1.0;

    fParticleGun->SetParticlePosition(G4ThreeVector(startX, startY, startZ));
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(px, py, pz));
}
Generator::~Generator() {
    delete fParticleGun;
}

void Generator::GeneratePrimaries(G4Event* anEvent) {
    fParticleGun->GeneratePrimaryVertex(anEvent);
}

