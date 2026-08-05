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
#include "G4GenericMessenger.hh"

#include "detector.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
	MyDetectorConstruction();
	~MyDetectorConstruction();

	G4LogicalVolume *GetScoringVolume() const { return fScoringVolume; }
    
	void SetWLSSize(G4double val);
    void SetWLSThickness(G4double val);
	void SetWLSGap(G4double val);
    
    G4double GetWLSSize() const { return fWLSSize; }
    G4double GetWLSThickness() const { return fWLSThickness; }
	G4double GetWLSGap() const { return fWLSGap; }

	void SetWLSTimeConstant(G4double val);
	G4double GetWLSTimeConstant() const { return fWLSTimeConstant; }

	virtual G4VPhysicalVolume *Construct();
	void ConstructWLS();

private:
	G4LogicalVolume *logicLiner, *logicWater, *logicWorld, *logicWLS, *logicPMTSphere, *logicWLSMirror;
	G4Box *solidLiner, *solidWater, *solidWorld, *solidWLS, *solidMirrorWLSBox;
	G4VPhysicalVolume *physLiner, *physWater, *physWorld, *physWLS, *physPMTSphere, *physWLSMirror;
	G4Sphere *solidPMTSphere;
	G4VSolid *WLSMirrorSubtract, *WLS;

	G4double xWorld, yWorld, zWorld;
  G4Material *worldMat, *Air, *PMMA, *Water, *Polyethylene, *Glass, *Silver, *Aluminium;
  G4OpticalSurface *TyvekSurface, *linerSurface, *pmtCouplingSurface, *SilverCoatingSurface, *AluminiumCoatingSurface;
	G4LogicalSkinSurface *WLSMirrorSkin, *linerSkin;

	G4LogicalVolume *fScoringVolume;

	G4PhysicsFreeVector *wlsAbs, *wlsEmm, *wlsRef;

	G4GenericMessenger* fMessenger;
    static G4double fWLSSize; 
    static G4double fWLSThickness; 
	static G4double fWLSTimeConstant;
	static G4double fWLSGap;

	virtual void ConstructSDandField();
	void DefineMaterials();
};

#endif
