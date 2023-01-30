#include "construction.hh"


MyDetectorConstruction::MyDetectorConstruction()
{
    xWorld = 1*m;
    yWorld = 0.5*m;
    zWorld = 1*m;

    DefineMaterials();
}

MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterials()
{
    G4NistManager *nist = G4NistManager::Instance();  //the geant4 nist material database

	Polyethylene = new G4Material("Polyethylene", 1.200*g/cm3, 2); //defining material object
	Polyethylene->AddElement(nist->FindOrBuildElement("H"), 4); //adding the hydrogen element to object
	Polyethylene->AddElement(nist->FindOrBuildElement("C"), 2); //adding the Carbon element to object

	G4double energy[2] = {1.239841939*eV/0.9, 1.239841939*eV/0.2}; //momentum of photons usng conversion factor *eV / wavlength of red light and blue light

	G4double rindexPolyethylene[2] = {1.49, 1.49}; //refractive index
	G4double rindexWorld[2] = {1.333, 1.333}; //refractive index of the world volume (air)
	G4double rindexAir[2] = {1.0, 1.0};

    G4double WLS_energy[4] = {2.00*eV,2.87*eV,2.90*eV,3.47*eV};
    G4double WLS_rindex[4]={ 1.5, 1.5, 1.5, 1.5};
    G4double WLS_abslength[4]={9.00*m, 9.00*m, 0.1*m, 0.1*m};
    G4double WLS_emission[4]={1.0, 1.0, 0.0, 0.0};

	G4MaterialPropertiesTable *mptWLS = new G4MaterialPropertiesTable();
	mptWLS->AddProperty("RINDEX", WLS_energy, WLS_rindex, 4); //adding property to this object
	mptWLS->AddProperty("WLSABSLENGTH", WLS_energy, WLS_abslength, 4);
    mptWLS->AddProperty("WLSCOMPONENT", WLS_energy, WLS_emission, 4);
    mptWLS->AddConstProperty("WLSTIMECONSTANT", 0.5*ns);

	Polyethylene->SetMaterialPropertiesTable(mptWLS);

	worldMat = nist->FindOrBuildMaterial("G4_WATER"); //createing the world material (air)

	G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable(); //creating properites object for air
	mptWorld->AddProperty("RINDEX", energy, rindexWorld, 2); //adding property to object

	worldMat->SetMaterialPropertiesTable(mptWorld); //assigning properties object to world material object


	Air = nist->FindOrBuildMaterial("G4_AIR"); //createing the world material (air)

	G4MaterialPropertiesTable *mptAir= new G4MaterialPropertiesTable(); //creating properites object for air
	mptAir->AddProperty("RINDEX", energy, rindexAir, 2); //adding property to object

	Air->SetMaterialPropertiesTable(mptAir); //assigning properties object to world material object


	mirrorSurface = new G4OpticalSurface("mirrorSurface");

    mirrorSurface->SetType(dielectric_metal);
    mirrorSurface->SetFinish(polished);
    mirrorSurface->SetModel(glisur);

    G4double reflectivity[2] = {0.9, 0.9};
    G4double pp[2] = { 2.0 * eV, 3.5 * eV };
    G4double efficiency[2]   = { 0.0, 0.0 };

    G4MaterialPropertiesTable *mptMirror = new G4MaterialPropertiesTable();
    mptMirror->AddProperty("REFLECTIVITY", pp, reflectivity, 2);
    mptMirror->AddProperty("EFFICIENCY", pp, efficiency, 2);

    mirrorSurface->SetMaterialPropertiesTable(mptMirror);
}

void MyDetectorConstruction::ConstructWLS()
{

	G4RotationMatrix* RotationPMTSphere = new G4RotationMatrix();
	RotationPMTSphere->rotateX(90*deg);


	solidPMTSphere = new G4Sphere("solidPMTSphere", 0., 0.127*m, 0., 2*pi, 0., 0.5*pi);  //setting the dimensions for the detectors

	logicPMTSphere = new G4LogicalVolume(solidPMTSphere, Air, "logicPMTSphere"); //creating the detectors, with same material as world

	physPMTSphere = new G4PVPlacement(RotationPMTSphere, G4ThreeVector(0., -0.5*m + 0.127*m+ 0.01*m, 0.), logicPMTSphere, "physPMTSphere", logicWorld, false, 0, true);



	solidPMTCylinder = new G4Tubs("solidPMTCylinder", 0., 0.127*m, 0.127*m, 0., 2*pi);  //setting the dimensions for the detectors


	solidPMTBox = new G4Box("solidPMTBox", 0.127*m, 0.0635*m, 0.127*m);


    G4RotationMatrix* RotationPMT = new G4RotationMatrix();
	RotationPMT->rotateX(90*deg);

	G4VSolid* intersect    = new G4IntersectionSolid("intersect", solidPMTBox, solidPMTCylinder, RotationPMT, G4ThreeVector(0., -0.0635*m, 0.));

	logicPMT = new G4LogicalVolume(intersect, Air, "logicPMT"); //creating the detectors, with same material as world

	physPMT = new G4PVPlacement(0, G4ThreeVector(0., -0.5*m + 0.0635*m+ 0.01*m, 0.), logicPMT, "physPMT", logicWorld, false, 0, true);


	G4RotationMatrix* RotationWLS = new G4RotationMatrix();
	RotationWLS->rotateX(90*deg);

	solidWLS = new G4Box("solidWLS", 0.4*m, 0.4*m, 0.0635*m); //setting the dimensions for the radiator



    G4VSolid* subtract  = new G4SubtractionSolid("Box-Cylinder", solidWLS, solidPMTCylinder, 0, G4ThreeVector(0., 0, 0.));

    logicWLS = new G4LogicalVolume(subtract, Polyethylene, "logicWLS"); //creating the aergoel radiator from dimensions

    //physWLS = new G4PVPlacement(RotationWLS, G4ThreeVector(0., -0.5*m + 0.0635*m + 0.01*m, 0.), logicWLS, "physWLS", logicWorld, false, 0, true); //placing radiator into world volume


    fScoringVolume = logicWLS;


    //G4LogicalSkinSurface *skin = new G4LogicalSkinSurface("skin", logicWLS, mirrorSurface);

    //G4Box* solidAir = new G4Box("solidAir", xWorld, yWorld, 0.1*m);

    //G4LogicalVolume* logicAir = new G4LogicalVolume(solidAir,Air,"logicAir");

    //G4PVPlacement* physAir = new G4PVPlacement(RotationWLS, G4ThreeVector(0, 0.4*m, 0), logicAir, "physAir", logicWorld, false, 0, true);




}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{

    solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld); //setting up world volume as a box

	logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld"); //creating the world as dimensions from box and material

	physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true); //placing the world into geant 4 sim

    ConstructWLS();

	return physWorld; //returning the world, including everything inside
}

void MyDetectorConstruction::ConstructSDandField()
{
    MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector"); //creating a sensitive detector object

    if(logicPMTSphere != NULL)
        logicPMTSphere->SetSensitiveDetector(sensDet); //assigning the detectors as senstive detectors (it generates a hit when a track traverses its volume)s

    if(logicPMTCylinder != NULL)
        logicPMT->SetSensitiveDetector(sensDet);



}

