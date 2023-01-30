#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
	fParticleGun = new G4ParticleGun(1);
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
	delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4String particleName="e-";
	G4ParticleDefinition *particle = particleTable->FindParticle("e-");
	G4ThreeVector mom = G4RandomDirection();

	G4double x0 = 1*m * (G4UniformRand()-1);
    G4double y0 = 0.5*m * (G4UniformRand()-0.5);
    G4double z0 = 1*m * (G4UniformRand()-1);

    fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));

	//G4ThreeVector pos(0, 0, 0);
	//G4ThreeVector mom(0, -1., 0.);

	//fParticleGun->SetParticlePosition(pos);
	fParticleGun->SetParticleMomentumDirection(mom);
	fParticleGun->SetParticleMomentum(4*GeV);
	fParticleGun->SetParticleDefinition(particle);

	fParticleGun->GeneratePrimaryVertex(anEvent);
}

