#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4VSolid.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4IntersectionSolid.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4OpWLS.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4SubtractionSolid.hh"
#include "G4SDManager.hh"

#include "detector.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
	MyDetectorConstruction();
	~MyDetectorConstruction();

	G4LogicalVolume *GetScoringVolume() const { return fScoringVolume; }

	virtual G4VPhysicalVolume *Construct();
	void ConstructWLS();

private:
	G4LogicalVolume  *logicWorld, *logicWLS, *logicPMTSphere, *logicPMTCylinder, *logicPMTBox, *logicPMT;
	G4Box *solidWorld, *solidWLS, *solidPMTBox;
	G4VPhysicalVolume  *physWorld, *physWLS, *physPMTSphere, *physPMTCylinder, *physPMTBox, *physPMT;
	G4Sphere *solidPMTSphere;
	G4Tubs *solidPMTCylinder;

	G4double xWorld, yWorld, zWorld;
	G4Material *worldMat, *Air, *Polyethylene, *Water;
	G4OpticalSurface *mirrorSurface;

	G4LogicalVolume *fScoringVolume;

	virtual void ConstructSDandField();
	void DefineMaterials();
};

#endif
