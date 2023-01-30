#include "physics.hh"

MyPhysicsList::MyPhysicsList()
{
	RegisterPhysics (new G4EmStandardPhysics_option4());
	RegisterPhysics (new G4OpticalPhysics());
	RegisterPhysics (new G4DecayPhysics());

	G4VModularPhysicsList* physicsList = new FTFP_BERT;

    auto opticalParams = G4OpticalParameters::Instance();
    opticalParams->SetBoundaryInvokeSD(true);

    G4Cerenkov* theCerenkovProcess = new G4Cerenkov("Cerenkov");
    theCerenkovProcess->SetTrackSecondariesFirst(true);
    G4int MaxNumPhotons = 100;
    theCerenkovProcess->SetMaxNumPhotonsPerStep(MaxNumPhotons);
    theCerenkovProcess->SetMaxBetaChangePerStep(10.0);
}

MyPhysicsList::~MyPhysicsList()
{}
