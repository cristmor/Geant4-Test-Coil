#include "Initialization.hh"

Initialization::Initialization (Detector* detConstruction)
    : G4VUserActionInitialization(), fDetConstruction(detConstruction){
}

Initialization::~Initialization() {
}

void Initialization::Build() const {
    SetUserAction(new Generator());
    auto eventAction = new Event();
    SetUserAction(eventAction);
    SetUserAction(new Stepping(fDetConstruction, eventAction));
}  
