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
	G4String particleName="e-"; //electron=e-, proton = p+, muon = mu-
	G4double particleEnergy = 0.8*GeV;
	G4ParticleDefinition *particle = particleTable->FindParticle(particleName);

	G4ThreeVector pos(0., 0., 0.); //origin point, center of world volume
	G4ThreeVector mom(0., 0., 1.); //going in the positive Z-direction

	fParticleGun->SetParticlePosition(pos);
	fParticleGun->SetParticleMomentumDirection(mom);
	fParticleGun->SetParticleMomentum(particleEnergy);
	fParticleGun->SetParticleDefinition(particle);

	fParticleGun->GeneratePrimaryVertex(anEvent);
}

