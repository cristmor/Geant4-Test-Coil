#include "Generator.hh"

Generator::Generator()
    : G4VUserPrimaryGeneratorAction(), fParticleGun(nullptr){
    /*
    fParticleGun = new G4ParticleGun(1);

    auto particleDefinition = G4OpticalPhoton::OpticalPhotonDefinition();
    // auto particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("e-");
    fParticleGun->SetParticleDefinition(particleDefinition);
    fParticleGun->SetParticleEnergy(100.0*keV);

    G4double startX = 0*cm;
    G4double startY = 0*cm;
    G4double startZ = 0*cm;

    G4double px = 0.0;
    G4double py = 0.05;
    G4double pz = -0.95;

    fParticleGun->SetParticlePosition(G4ThreeVector(startX, startY, startZ));
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(px, py, pz));
    */
}
Generator::~Generator() {
    delete fParticleGun;
}

void Generator::GeneratePrimaries(G4Event* anEvent) {
    G4ParticleGun* fParticleGun = new G4ParticleGun(1);

    auto particleDefinition = G4OpticalPhoton::OpticalPhotonDefinition();
    fParticleGun->SetParticleDefinition(particleDefinition);

    // Set particle energy
    fParticleGun->SetParticleEnergy(100.0*keV);

    // Set particle position
    fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 5.0 * cm));

    // Randomize direction around pz = -1.0
    G4double theta = G4RandGauss::shoot(0., 0.1); // Narrow Gaussian spread around z-axis
    G4double phi = G4UniformRand() * 2. * CLHEP::pi;

    G4double px = std::sin(theta) * std::cos(phi);
    G4double py = std::sin(theta) * std::sin(phi);
    G4double pz = -std::cos(theta); // Ensure the main direction is along -z

    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(px, py, pz).unit());
    fParticleGun->GeneratePrimaryVertex(anEvent);

    delete fParticleGun;
}

