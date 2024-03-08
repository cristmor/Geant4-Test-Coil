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
// TODO List:
// 1. Add better data taking and add some data structure
// 2. Do runs with 5 GeV electrons
/// \file G4Coil.cc
/// \brief Main program of the  example
#include <iostream>
#include <chrono>

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#include "G4RunManagerFactory.hh"

#include "G4UImanager.hh"
#include "G4UIcommand.hh"
// #include "FTFP_BERT.hh"

#include "Randomize.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "QGSP_BERT.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4OpticalPhysics.hh"

#include "TROOT.h"
#include "TThread.h"

#include "CoilTree.hh"


int main(int argc,char** argv)
{
    // Enable thread safety for ROOT
    ROOT::EnableThreadSafety();

    auto coilTree = new CoilTree();

    // Stop the timer
    auto start = std::chrono::high_resolution_clock::now();

    // Evaluate arguments
    G4String macro;
    G4String session;
#ifdef G4MULTITHREADED
    G4int nThreads = 0;
#endif
    for( G4int i=1; i<argc; i=i+2 )
    {
        if ( G4String(argv[i]) == "-m" ) 
        {
            macro = argv[i+1];
            coilTree->SetParameters(macro);
        }
    else if ( G4String(argv[i]) == "-u" ) session = argv[i+1];
#ifdef G4MULTITHREADED
    else if( G4String(argv[i]) == "-t" )
    {
        nThreads = G4UIcommand::ConvertToInt(argv[i+1]);
    }
#endif
  }  
  
	// Detect interactive mode (if no macro provided) and define UI session
	//
	G4UIExecutive* ui = nullptr;
	if(  !coilTree->GetParameters().batchMode )
	{
		ui = new G4UIExecutive(argc, argv, session);
	}

	// Optionally: choose a different Random engine...
	//
	// G4Random::setTheEngine(new CLHEP::MTwistEngine);

	// Construct the default run manager
	//
	auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::MT);
#ifdef G4MULTITHREADED
	if( coilTree->GetParameters().nThreads > 0 )
	{ 
		runManager->SetNumberOfThreads(coilTree->GetParameters().nThreads);
	}  
#endif

	// Set mandatory initialization classes
	//
	auto detConstruction = new DetectorConstruction(coilTree);
	runManager->SetUserInitialization(detConstruction);

	// auto physicsList = new FTFP_BERT;
	auto physicsList = new QGSP_BERT;
	physicsList->ReplacePhysics(new G4EmStandardPhysics_option4());
	G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
	physicsList->RegisterPhysics(opticalPhysics);


	runManager->SetUserInitialization(physicsList);
	auto actionInitialization = new ActionInitialization(detConstruction, coilTree);
	runManager->SetUserInitialization(actionInitialization);

	// Initialize visualization
	//
	auto visManager = new G4VisExecutive;
	// G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
	// G4VisManager* visManager = new G4VisExecutive("Quiet");
	visManager->Initialize();

	// Get the pointer to the User Interface manager
	auto UImanager = G4UImanager::GetUIpointer();

	// Process macro or start UI session
	//
	if ( coilTree->GetParameters().batchMode )
	{
		// batch mode
		UImanager->ApplyCommand("/run/initialize");
		UImanager->ApplyCommand("/gun/particle "+coilTree->GetParameters().particle);
		UImanager->ApplyCommand("/gun/energy "+std::to_string(coilTree->GetParameters().energy)+" GeV");
		UImanager->ApplyCommand("/run/beamOn "+std::to_string(coilTree->GetParameters().nEvents));
		UImanager->ApplyCommand("/event/verbose 0");
		UImanager->ApplyCommand("/run/verbose 0");
	}
	else 
	{  
	// interactive mode : define UI session
	UImanager->ApplyCommand("/control/execute init_vis.mac");
	if(ui->IsGUI())
	{
		UImanager->ApplyCommand("/control/execute gui.mac");
	}
		ui->SessionStart();
		delete ui;
	}

	// Job termination
	// Free the store: user actions, physics_list and detector_description are
	// owned and deleted by the run manager, so they should not be deleted 
	// in the main() program !

	delete coilTree;
	delete visManager;
	delete runManager;

	// Stop the timer
	auto end = std::chrono::high_resolution_clock::now();

	// Calculate the duration
	std::chrono::duration<double> duration = end - start;
	double durationInSeconds = duration.count();
	std::cout << "Timer: " << durationInSeconds << " seconds." << std::endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
