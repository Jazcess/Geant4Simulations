#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"
#include "detector.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4GeometryManager.hh"
#include "G4RunManagerKernel.hh"
#include "G4UImanager.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
	MyDetectorConstruction();
	~MyDetectorConstruction();

	virtual G4VPhysicalVolume *Construct();

private:
	G4LogicalVolume *logicDetector;
	G4VPhysicalVolume *physDetector;
	void DefineMaterials();
	virtual void ConstructSDandField();

	G4double xWorld, yWorld, zWorld, xRadiator1, yRadiator1, zRadiator1, rRadiator1, dRadiator1, xRadiator2, yRadiator2, zRadiator2, rRadiator2, dRadiator2, xPixel, yPixel, zPixel, posX, posY, posZ, xDet, yDet;

	G4int nCols, nRows, mRadiator1, mWorld, mRadiator2;

	G4GenericMessenger *dMessenger, *wMessenger, *r1Messenger, *mMessenger, *r2Messenger;

	G4Material *worldMat, *radiatorMat1, *SiO2, *H2O, *radiatorMat2, *B2O3;
	G4Element *C;

	G4MaterialPropertiesTable *mptWorld, *mptRadiator1, *mptRadiator2;

	G4RunManagerKernel *kernel;


};

#endif
