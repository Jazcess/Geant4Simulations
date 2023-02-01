#include "construction.hh"


MyDetectorConstruction::MyDetectorConstruction()
{
    dMessenger = new G4GenericMessenger(this, "/detector/", "Detector Construction");
    wMessenger = new G4GenericMessenger(this, "/world/", "World Construction");
    aMessenger = new G4GenericMessenger(this, "/aerogel/", "Aerogel Construction");

    dMessenger->DeclareProperty("nCols", nCols, "Number of columns");
    dMessenger->DeclareProperty("nRows", nRows, "Number of rows");
    dMessenger->DeclareProperty("xDet", xDet, "X length of detector");
    dMessenger->DeclareProperty("yDet", yDet, "Y length of detector");
    wMessenger->DeclareProperty("xWorld", xWorld, "X length of world");
    wMessenger->DeclareProperty("yWorld", yWorld, "Y length of world");
    wMessenger->DeclareProperty("zWorld", zWorld, "Z length of world");
    aMessenger->DeclareProperty("xAerogel", xAerogel, "X length of aerogel");
    aMessenger->DeclareProperty("yAerogel", yAerogel, "Y length of aerogel");
    aMessenger->DeclareProperty("zAerogel", zAerogel, "Z length of aerogel");
    aMessenger->DeclareProperty("dAerogel", dAerogel, "Distance from aerogel to detectors");
    aMessenger->DeclareProperty("rAerogel", rAerogel, "Refractive index of aerogel");

    nCols = 10;
    nRows = 10;

    xWorld = 0.05*m;
    yWorld = 0.05*m;
    zWorld = 0.05*m;

    xAerogel = 0.02*m;
    yAerogel = 0.02*m;
    zAerogel = 0.005*m;
    dAerogel = 0.04*m;
    rAerogel = 1.1;

    xDet = 0.012*m;
    yDet = 0.012*m;

    xPixel = xDet/nCols;
    yPixel = yDet/nRows;
    zPixel = 0.001*m;

}

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

	G4double rindexAerogel[2] = {rAerogel, rAerogel}; //refractive index of aerogel
	G4double rindexWorld[2] = {1.0, 1.0}; //refractive index of the world volume (air)

	G4MaterialPropertiesTable *mptAerogel = new G4MaterialPropertiesTable(); //creating aerogel material properties object
	mptAerogel->AddProperty("RINDEX", energy, rindexAerogel, 2); //adding property to this object

	Aerogel->SetMaterialPropertiesTable(mptAerogel); //assigning the Aergoel object the properties from the previous object created

	G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR"); //createing the world material (air)

	G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable(); //creating properites object for air
	mptWorld->AddProperty("RINDEX", energy, rindexWorld, 2); //adding property to object

	worldMat->SetMaterialPropertiesTable(mptWorld); //assigning properties object to world material object




	G4Box *solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld); //setting up world volume as a box

	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld"); //creating the world as dimensions from box and material (Air)

	G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true); //placing the world into geant 4 sim

	G4Box *solidRadiator = new G4Box("solidRadiator", xAerogel, yAerogel, zAerogel); //setting the dimensions for the radiator

	G4LogicalVolume *logicRadiator = new G4LogicalVolume(solidRadiator, Aerogel, "logicRadiator"); //creating the aergoel radiator from dimensions

	G4VPhysicalVolume *physRadiator = new G4PVPlacement(0, G4ThreeVector(0., 0., zWorld - dAerogel), logicRadiator, "physRadiator", logicWorld, false, 0, true); //placing radiator into world volume

	G4Box *solidDetector = new G4Box("solidDetector", xPixel, yPixel, zPixel);  //setting the dimensions for the detectors

	logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector"); //creating the detectors, with same material as world



	for(G4int i = 0; i < nCols; i++)
	{

		for(G4int j = 0; j < nRows; j++)
		{
		    posX = -xDet+(i*2*xPixel);

		    posY = -yDet+(j*2*yPixel);

		    posZ = zWorld - zPixel;

			G4VPhysicalVolume *physDetector= new G4PVPlacement(0, G4ThreeVector(posX, posY, posZ), logicDetector, "physDetector", logicWorld, false, j+i*nCols, true); //placing detectors into the world

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

