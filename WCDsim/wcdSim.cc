#include <iostream>

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "Randomize.hh"
#include <time.h>
#include <cstdlib> // ADDED: Required to read cluster environment variables

#include "construction.hh"
#include "physics.hh"
#include "action.hh"

int main(int argc, char** argv)
{
    // Choose the Random engine
    G4Random::setTheEngine(new CLHEP::RanecuEngine);

    // G4long seed = time(NULL); // Set the random seed to the system time
    G4long seed = 6;  //Keep the seed constant for debugging
    G4Random::setTheSeed(seed);

    #ifdef G4MULTITHREADED
        G4MTRunManager *runManager = new G4MTRunManager();
        
        // ---> SMART THREAD ASSIGNMENT FOR CLUSTERS <---
        G4int nThreads = 4; 
        
        // Check if we are running inside a Slurm job on the 'Alice' cluster
        if (const char* slurmCpus = std::getenv("SLURM_CPUS_PER_TASK")) {
            nThreads = std::atoi(slurmCpus);
            std::cout << "--- SLURM JOB DETECTED --- Allocating " << nThreads << " threads." << std::endl;
        } else {
            std::cout << "--- LOCAL RUN DETECTED --- Defaulting to " << nThreads << " threads." << std::endl;
        }
        
        runManager->SetNumberOfThreads(nThreads); 
    #else
        G4RunManager *runManager = new G4RunManager();
    #endif

    // Initialization classes
    runManager->SetUserInitialization(new MyDetectorConstruction());
    runManager->SetUserInitialization(new MyPhysicsList());
    runManager->SetUserInitialization(new MyActionInitialization());
    
    // Geometry and Physics are built here. Thread count MUST be set before this line!
    runManager->Initialize();

    G4UIExecutive  *ui = 0;

    if(argc == 1)
    {
        ui = new G4UIExecutive(argc, argv);
    }

    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();

    G4UImanager * UImanager = G4UImanager::GetUIpointer();

    if(ui)
    {
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
    }
    else
    {
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }

    // Clean up
    delete visManager;
    delete runManager;

    return 0;
}