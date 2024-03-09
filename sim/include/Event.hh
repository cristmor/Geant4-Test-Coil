#pragma once

#include <iostream>
#include <iomanip>

#include "G4UserEventAction.hh"
#include "globals.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"
#include "G4Threading.hh"

#include "Randomize.hh"

class Event : public G4UserEventAction
{
public:
    Event();
    virtual ~Event();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

};

    
