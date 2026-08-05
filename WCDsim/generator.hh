#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4RandomDirection.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4UserEventAction.hh"
#include "G4GenericMessenger.hh"

class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
	MyPrimaryGenerator();
	~MyPrimaryGenerator();

	virtual void GeneratePrimaries(G4Event*);
private:
	G4ParticleGun *fParticleGun;
	
	G4GenericMessenger* fMessenger;

    // Variable to store the X position from the macro
    G4double fXPos;
};

#endif
