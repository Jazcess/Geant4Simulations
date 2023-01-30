#include "construction.hh"


MyDetectorConstruction::MyDetectorConstruction()
{}

MyDetectorConstruction::~MyDetectorConstruction()
{}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
	G4NistManager *nist = G4NistManager::Instance();  //the geant4 nist material database

	G4Material *SiO2 = new G4Material("SiO2", 2.201*g/cm3, 2); //defining the silicone dioxide material object
	SiO2->AddElement(nist->FindOrBuildElement("Si"), 1);  //adding the silicone element to the object
	SiO2->AddElement(nist->FindOrBuildElement("O"), 2); //adding the oxygen element to the object

	G4Material *H2O = new G4Material("H2O", 1.000*g/cm3, 2); //defining h20 material object
	H2O->AddElement(nist->FindOrBuildElement("H"), 2); //adding the hydrogen element to object
	H2O->AddElement(nist->FindOrBuildElement("O"), 1); //adding the oxygen element to object

	G4Element *C = nist->FindOrBuildElement("C"); //creating the carbon material object

	G4Material *Aerogel = new G4Material("Aerogel", 0.200*g/cm3, 3); //creating the aerogel material object
	Aerogel->AddMaterial(SiO2, 62.5*perCent);  //adding the silione dioxide material to aerogel object
	Aerogel->AddMaterial(H2O, 37.4*perCent);  //adding the h20 material to aergoel object
	Aerogel->AddElement(C, 0.1*perCent);  //adding the carbon material to aerogel object

	G4double energy[2] = {1.239841939*eV/0.9, 1.239841939*eV/0.2}; //momentum of photons usng conversion factor *eV / wavlength of red light and blue light

	G4double rindexAerogel[2] = {1.1, 1.1}; //refractive index of aerogel
	G4double rindexWorld[2] = {1.0, 1.0}; //refractive index of the world volume (air)

	G4MaterialPropertiesTable *mptAerogel = new G4MaterialPropertiesTable(); //creating aerogel material properties object
	mptAerogel->AddProperty("RINDEX", energy, rindexAerogel, 2); //adding property to this object

	Aerogel->SetMaterialPropertiesTable(mptAerogel); //assigning the Aergoel object the properties from the previous object created
	
	G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR"); //createing the world material (air)

	G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable(); //creating properites object for air
	mptWorld->AddProperty("RINDEX", energy, rindexWorld, 2); //adding property to object

	worldMat->SetMaterialPropertiesTable(mptWorld); //assigning properties object to world material object

	G4Box *solidWorld = new G4Box("solidWorld", 0.5*m, 0.5*m, 0.5*m); //setting up world volume as a box

	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld"); //creating the world as dimensions from box and material (Air)

	G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true); //placing the world into geant 4 sim

	G4Box *solidRadiator = new G4Box("solidRadiator", 0.4*m, 0.4*m, 0.01*m); //setting the dimensions for the radiator

	G4LogicalVolume *logicRadiator = new G4LogicalVolume(solidRadiator, Aerogel, "logicRadiator"); //creating the aergoel radiator from dimensions

	G4VPhysicalVolume *physRadiator = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.25*m), logicRadiator, "physRadiator", logicWorld, false, 0, true); //placing radiator into world volume

	G4Box *solidDetector = new G4Box("solidDetector", 0.005*m, 0.005*m, 0.01*m);  //setting the dimensions for the detectors

	logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector"); //creating the detectors, with same material as world

	

	for(G4int i = 0; i < 100; i++)
	{

		for(G4int j = 0; j < 100; j++)
		{
			
			G4VPhysicalVolume *physDetector= new G4PVPlacement(0, G4ThreeVector(-0.5*m+(i+0.5)*m/100, -0.5*m+(j+0.5)*m/100, 0.49*m), logicDetector, "physDetector", logicWorld, false, j+i*100, true); //placing 100*100 detectors into the world

		}
	}




	return physWorld; //returning the world, including everything inside
}

void MyDetectorConstruction::ConstructSDandField()
{
    MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector"); //creating a sensitive detector object

    if(logicDetector != NULL)
        logicDetector->SetSensitiveDetector(sensDet); //assigning the detectors as senstive detectors (it generates a hit when a track traverses its volume)
}
	
