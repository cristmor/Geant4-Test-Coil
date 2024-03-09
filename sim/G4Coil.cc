#include <iostream>
#include <chrono>

#include "Detector.hh"
#include "Initialization.hh"

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

int main(int argc,char** argv)
{
    // Enable thread safety for ROOT
    // ROOT::EnableThreadSafety();

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
            // coilTree->SetParameters(macro);
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
	if( true )
	{
		ui = new G4UIExecutive(argc, argv, session);
	}

	// Construct the default run manager
	//
	auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::MT);
#ifdef G4MULTITHREADED
	if( true )
	{ 
		runManager->SetNumberOfThreads(4);
	}  
#endif

	// Set mandatory initialization classes
	//
	auto detConstruction = new Detector();
	runManager->SetUserInitialization(detConstruction);

	// auto physicsList = new FTFP_BERT;
	auto physicsList = new QGSP_BERT;
	physicsList->ReplacePhysics(new G4EmStandardPhysics_option4());
	G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
	physicsList->RegisterPhysics(opticalPhysics);
	runManager->SetUserInitialization(physicsList);


	auto actionInitialization = new Initialization(detConstruction);
	runManager->SetUserInitialization(actionInitialization);

	// Initialize visualization
	//
	auto visManager = new G4VisExecutive;
	visManager->Initialize();

	auto UImanager = G4UImanager::GetUIpointer();

	// Process macro or start UI session
	//
	// if ( false )
	// {
	// 	// batch mode
	// 	UImanager->ApplyCommand("/run/initialize");
	// 	UImanager->ApplyCommand("/event/verbose 0");
	// 	UImanager->ApplyCommand("/run/verbose 0");
	// }
	// else 
	// {  
		// interactive mode : define UI session
		UImanager->ApplyCommand("/control/execute init_vis.mac");
		if(ui->IsGUI())
		{
			UImanager->ApplyCommand("/control/execute gui.mac");
		}
		ui->SessionStart();
		delete ui;
	// }

	delete visManager;
	delete runManager;

	// Stop the timer
	auto end = std::chrono::high_resolution_clock::now();

	// Calculate the duration
	std::chrono::duration<double> duration = end - start;
	double durationInSeconds = duration.count();
	std::cout << "Timer: " << durationInSeconds << " seconds." << std::endl;

}