#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Ellipsoid.hh"
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
	G4LogicalVolume *logicLiner, *logicWater, *logicWorld, *logicWLS, *logicPMTSphere, *logicWLSMirror;
	G4Box *solidLiner, *solidWater, *solidWorld, *solidWLS, *solidMirrorWLSBox;
	G4VPhysicalVolume *physLiner, *physWater, *physWorld, *physWLS, *physPMTSphere, *physWLSMirror;
	G4Sphere *solidPMTSphere;
	G4VSolid *WLSMirrorSubtract, *WLS;

	G4double xWorld, yWorld, zWorld;
	G4Material *worldMat, *Air, *PMMA, *Water, *Polyethylene, *Glass;
	G4OpticalSurface *mirrorSurface, *linerSurface;
	G4LogicalSkinSurface *WLSMirrorSkin, *linerSkin;

	G4LogicalVolume *fScoringVolume;

	G4PhysicsFreeVector *wlsAbs, *wlsEmm, *wlsRef;

	virtual void ConstructSDandField();
	void DefineMaterials();
};

#endif
