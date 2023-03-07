#include "construction.hh"


MyDetectorConstruction::MyDetectorConstruction()
{

    nCols = 10;
    nRows = 10;

    xWorld = 0.05*m;
    yWorld = 0.05*m;
    zWorld = 0.05*m;

    xRadiator1 = 0.02*m;
    yRadiator1 = 0.02*m;
    zRadiator1 = 0.0025*m;
    dRadiator1 = 0.03*m;

    xRadiator2 = 0.02*m;
    yRadiator2 = 0.02*m;
    zRadiator2 = 0.005*m;
    dRadiator2 = 0.007*m;

    xDet = 0.02*m;
    yDet = 0.02*m;

    mRadiator1 = 2; // 1 = water, 2 = aerogel
    mRadiator2 = 3; // 3 = glass
    mWorld = 4; // 4 = air

    xPixel = xDet/nRows;
    yPixel = yDet/nCols;
    zPixel = 0.001*m;

    

    dMessenger = new G4GenericMessenger(this, "/detector/", "Detector Construction");
    wMessenger = new G4GenericMessenger(this, "/world/", "World Construction");
    r1Messenger = new G4GenericMessenger(this, "/Radiator1/", "Radiator1 Construction");
    r2Messenger = new G4GenericMessenger(this, "/Radiator2/", "Radiator2 Construction");
    mMessenger = new G4GenericMessenger(this, "/materials/", "Materials");

    dMessenger->DeclareProperty("Columns", nCols, "Number of columns");
    dMessenger->DeclareProperty("Rows", nRows, "Number of rows");
    dMessenger->DeclareProperty("XDetector", xDet, "X length of detector");
    dMessenger->DeclareProperty("YDetector", yDet, "Y length of detector");

    wMessenger->DeclareProperty("xWorld", xWorld, "X length of world");
    wMessenger->DeclareProperty("yWorld", yWorld, "Y length of world");
    wMessenger->DeclareProperty("zWorld", zWorld, "Z length of world");

    r1Messenger->DeclareProperty("xRadiator1", xRadiator1, "X length of Radiator1");
    r1Messenger->DeclareProperty("yRadiator1", yRadiator1, "Y length of Radiator1");
    r1Messenger->DeclareProperty("zRadiator1", zRadiator1, "Z length of Radiator1");
    r1Messenger->DeclareProperty("dRadiator1", dRadiator1, "Distance from Radiator1 to detectors");


    r2Messenger->DeclareProperty("xRadiator2", xRadiator2, "X length of Radiator2");
    r2Messenger->DeclareProperty("yRadiator2", yRadiator2, "Y length of Radiator2");
    r2Messenger->DeclareProperty("zRadiator2", zRadiator2, "Z length of Radiator2");
    r2Messenger->DeclareProperty("dRadiator2", dRadiator2, "Distance from Radiator2 to detectors");



    mMessenger->DeclareProperty("mRadiator1", mRadiator1, "Radiator 1 material");
    mMessenger->DeclareProperty("mRadiator2", mRadiator2, "Radiator 2 material");
    mMessenger->DeclareProperty("mWorld", mWorld, "World material");


    DefineMaterials();

}

MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterials()
{
    G4NistManager *nist = G4NistManager::Instance();  //the geant4 nist material database

	SiO2 = new G4Material("SiO2", 2.201*g/cm3, 2); //defining the silicone dioxide material object
	SiO2->AddElement(nist->FindOrBuildElement("Si"), 1);  //adding the silicone element to the object
	SiO2->AddElement(nist->FindOrBuildElement("O"), 2); //adding the oxygen element to the object

	H2O = new G4Material("H2O", 1.000*g/cm3, 2); //defining h20 material object
	H2O->AddElement(nist->FindOrBuildElement("H"), 2); //adding the hydrogen element to object
	H2O->AddElement(nist->FindOrBuildElement("O"), 1); //adding the oxygen element to object

	C = nist->FindOrBuildElement("C"); //creating the carbon material object

	B2O3 = new G4Material("B2O3", 2.46*g/cm3, 2);
	B2O3->AddElement(nist->FindOrBuildElement("B"), 2);
	B2O3->AddElement(nist->FindOrBuildElement("O"), 3);

	G4double energy[2] = {1.239841939*eV/0.9, 1.239841939*eV/0.2}; //momentum of photons usng conversion factor *eV / wavlength of red light and blue light

	G4double rindexAerogel[2] = {1.1, 1.1}; //refractive index of aerogel
	G4double rindexAir[2] = {1.0, 1.0}; //refractive index of the world volume (air)
	G4double rindexWater[2] = {1.333, 1.333}; //refractive index of the world volume (water)
	G4double rindexGlass[2] = {1.5, 1.5}; //refractive index of glass


    if(mRadiator1 == 1)
    {
        radiatorMat1 = nist->FindOrBuildMaterial("G4_WATER"); //createing the radiator material (water)
	    mptRadiator1 = new G4MaterialPropertiesTable(); //creating properites object for water
	    mptRadiator1->AddProperty("RINDEX", energy, rindexWater, 2); //adding property to object
	    radiatorMat1->SetMaterialPropertiesTable(mptRadiator1); //assigning properties object to radiator material object

    }
    if(mRadiator1 == 2)
    {

        radiatorMat1 = new G4Material("Aerogel", 0.200*g/cm3, 3); //creating the aerogel material object
	    radiatorMat1->AddMaterial(SiO2, 62.5*perCent);  //adding the silione dioxide material to aerogel object
	    radiatorMat1->AddMaterial(H2O, 37.4*perCent);  //adding the h20 material to aergoel object
	    radiatorMat1->AddElement(C, 0.1*perCent);  //adding the carbon material to aerogel object
	    mptRadiator1 = new G4MaterialPropertiesTable(); //creating aerogel material properties object
	    mptRadiator1->AddProperty("RINDEX", energy, rindexAerogel, 2); //adding property to this object
	    radiatorMat1->SetMaterialPropertiesTable(mptRadiator1); //assigning the Aergoel object the properties from the previous object created

    }
    if(mRadiator2 == 3)
    {

        radiatorMat2 = new G4Material("Glass", 2.23*g/cm3,2);//Creating glass
	    radiatorMat2->AddMaterial(SiO2, 70*perCent);
	    radiatorMat2->AddMaterial(B2O3, 30*perCent);
	    mptRadiator2 = new G4MaterialPropertiesTable(); //Creating glass material properties table
        mptRadiator2->AddProperty("RINDEX", energy, rindexGlass, 2); //adding property to this object

	    radiatorMat2->SetMaterialPropertiesTable(mptRadiator2); //setting material properties
    }


     if(mWorld == 4)
    {
        worldMat = nist->FindOrBuildMaterial("G4_AIR"); //createing the world material (air)
	    mptWorld = new G4MaterialPropertiesTable(); //creating properites object for air
	    mptWorld->AddProperty("RINDEX", energy, rindexAir, 2); //adding property to object
	    worldMat->SetMaterialPropertiesTable(mptWorld); //assigning properties object to world material object

    }



}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{

	G4Box *solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld); //setting up world volume as a box

	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld"); //creating the world as dimensions from box and material

	G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true); //placing the world into geant 4 sim



	G4Box *solidRadiator1 = new G4Box("solidRadiator1", xRadiator1, yRadiator1, zRadiator1); //setting the dimensions for the radiator

	G4LogicalVolume *logicRadiator1 = new G4LogicalVolume(solidRadiator1, radiatorMat1, "logicRadiator1"); //creating the aergoel radiator from dimensions

	G4VPhysicalVolume *physRadiator1 = new G4PVPlacement(0, G4ThreeVector(0., 0., zWorld-dRadiator1), logicRadiator1, "physRadiator1", logicWorld, false, 0, true); //placing radiator into world volume



	G4Box *solidRadiator2 = new G4Box("solidRadiator2", xRadiator2, yRadiator2, zRadiator2); //setting dimensions of glass radiator

	G4LogicalVolume *logicRadiator2 = new G4LogicalVolume(solidRadiator2, radiatorMat2, "logicRadiator2");

	G4VPhysicalVolume *physRadiator2 = new G4PVPlacement(0, G4ThreeVector(0., 0., zWorld-dRadiator2), logicRadiator2, "physRadiator2", logicWorld, false, 0, true);



	G4Box *solidDetector = new G4Box("solidDetector", xPixel, yPixel, zPixel);  //setting the dimensions for the detectors

	logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector"); //creating the detectors, with same material as world


	for(G4int i = 0; i < nRows; i++)
	{

		for(G4int j = 0; j <nCols; j++)
		{
		    posX = -xDet+(i*2*xDet/nRows) + xDet/nRows;

		    posY = -yDet+(j*2*yDet/nCols) + yDet/nCols;

		    posZ = zWorld - zPixel;

			physDetector= new G4PVPlacement(0, G4ThreeVector(posX, posY, posZ), logicDetector, "physDetector", logicWorld, false, j+i*nCols, true); //placing detectors into the world

			G4cout << posX << posY << posZ << G4endl;

		}
	}



	return physWorld; //returning the world, including everything inside
}

void MyDetectorConstruction::ConstructSDandField()
{

    MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector"); //creating a sensitive detector object

    if(logicDetector != NULL)
    {
       logicDetector->SetSensitiveDetector(sensDet); //assigning the detectors as senstive detectors (it generates a hit when a track traverses its volume)
    }

}

