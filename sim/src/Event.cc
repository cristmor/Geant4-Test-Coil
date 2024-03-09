#include "Event.hh"

Event::Event()
 : G4UserEventAction() {
}
Event::~Event() {
}

void Event::BeginOfEventAction(const G4Event* event) {
}
void Event::EndOfEventAction(const G4Event* event)
{
    int eventID = event->GetEventID();
    int printModulo = 100;

    if ( ( printModulo > 0 ) && eventID % printModulo == 0 ) {
        G4cout << " Event Total: " << eventID << G4endl;
    }
}