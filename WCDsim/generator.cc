#include "generator.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPrimaryGenerator.hh"
#include "G4MuonPlus.hh"
#include "Randomize.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0)
{
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);
  
  // Get the particle table and define the particle type
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* muon = G4MuonPlus::Definition();
  fParticleGun->SetParticleDefinition(muon);
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
  delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
{
  // Define the particle's energy
  G4double energy = 1.0 * GeV;
  
  // Set the direction to be vertically downwards along the Y-axis
  G4ThreeVector direction = G4ThreeVector(0.0, -1.0, 0.0);
  
  // Define the starting position
  // A safe practice is to get the world volume size from the detector construction.
  // For this example, we assume a world half-height of at least 1 meter in Y.
  G4double worldYHalfLength = 1.0 * m; // Replace with your actual world half-height

  // Set the position on the X-Z plane
  G4double xPos = 0.25 * m;
  G4double zPos = 0.0 * m;
  // Set the height to be at the top of the world in the Y direction
  G4double yPos = worldYHalfLength; 

  G4ThreeVector position = G4ThreeVector(xPos, yPos, zPos);

  // Set the particle's properties
  fParticleGun->SetParticleEnergy(energy);
  fParticleGun->SetParticleMomentumDirection(direction);
  fParticleGun->SetParticlePosition(position);

  // Generate the primary vertex
  fParticleGun->GeneratePrimaryVertex(anEvent);
}