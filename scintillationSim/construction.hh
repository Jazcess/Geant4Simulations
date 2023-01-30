#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "detector.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    MyDetectorConstruction();
    ~MyDetectorConstruction();

    G4LogicalVolume *GetScoringVolume() const { return fScoringVolume; }

    virtual G4VPhysicalVolume *Construct();
    void ConstructScintillator();


private:
    G4Box *solidWorld, *solidRadiator, *solidDetector, *solidScintillator, *solidAir;
    G4LogicalVolume *logicWorld, *logicRadiator, *logicDetector, *logicScintillator;
    G4VPhysicalVolume *physWorld, *physDetector, *physRadiator, *physScintillator;

    G4OpticalSurface *mirrorSurface;

    G4Material *worldMat, *NaI;
    G4Element *Na, *I;

    void DefineMaterials();
    virtual void ConstructSDandField();

    G4LogicalVolume *fScoringVolume;

    G4int nRows, nCols;

    G4double xWorld, yWorld, zWorld;

};

#endif
