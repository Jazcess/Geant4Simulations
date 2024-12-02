#include "construction.hh"


MyDetectorConstruction::MyDetectorConstruction()
{}


MyDetectorConstruction::~MyDetectorConstruction()
{}


G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
	G4NistManager *nist = G4NistManager::Instance();  //the geant4 nist material database

	G4double A, Z;

	// ------------------------------------------------------------------------
    // Elements
    // ------------------------------------------------------------------------


      G4Element* H  = new G4Element("Hydrogen","H",  Z=1.,  A=1.00794*g/mole);
      G4Element* C  = new G4Element("Carbon",  "C",  Z=6.,  A= 12.011 *g/mole);
      G4Element* N  = new G4Element("Nitrogen","N",  Z=7.,  A= 14.00674*g/mole);
      G4Element* O  = new G4Element("Oxygen",  "O",  Z=8.,  A= 15.9994*g/mole);
      G4Element* Na = new G4Element("Sodium",  "Na", Z=11., A= 22.989768*g/mole);
      G4Element* Si = new G4Element("Silicon", "Si", Z=14., A= 28.0855*g/mole);
      G4Element* Ar = new G4Element("Argon",   "Ar", Z=18., A= 39.948*g/mole);
      G4Element* I  = new G4Element("Iodine",  "I",  Z=53., A= 126.90447*g/mole);
      G4Element* Cs = new G4Element("Cesium",  "Cs", Z=55., A= 132.90543*g/mole);

	G4Material *SiO2 = new G4Material("SiO2", 2.201*g/cm3, 2); //defining the silicone dioxide material object
	SiO2->AddElement(Si, 1);  //adding the silicone element to the object
	SiO2->AddElement(O, 2); //adding the oxygen element to the object

	G4Material *H2O = new G4Material("H2O", 1.000*g/cm3, 2); //defining h20 material object
	H2O->AddElement(H, 2); //adding the hydrogen element to object
	H2O->AddElement(O, 1); //adding the oxygen element to object

	G4Material *Aerogel = new G4Material("Aerogel", 0.200*g/cm3, 3); //creating the aerogel material object
	Aerogel->AddMaterial(SiO2, 62.5*perCent);  //adding the silione dioxide material to aerogel object
	Aerogel->AddMaterial(H2O, 37.4*perCent);  //adding the h20 material to aergoel object
	Aerogel->AddElement(C, 0.1*perCent);  //adding the carbon material to aerogel object

	// quartz (SiO2, crystalline)
    G4Material* Quartz = new G4Material("Quartz", 2.64 *g/cm3, 2);
    Quartz-> AddElement(Si,1);
    Quartz-> AddElement(O, 2);

    G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR"); //createing the world material (air)



	G4double energy[2] = {1.239841939*eV/0.9, 1.239841939*eV/0.2}; //momentum of photons usng conversion factor *eV / wavlength of red light and blue light

	G4double rindexAerogel[2] = {1.1, 1.1}; //refractive index of aerogel
	G4double rindexQuartz[2] = {1.54, 1.54}; //refractive index of aerogel
	G4double rindexWorld[2] = {1.0, 1.0}; //refractive index of the world volume (air)


    //Material Property Tables
	G4MaterialPropertiesTable *mptAerogel = new G4MaterialPropertiesTable(); //creating aerogel material properties object
	mptAerogel->AddProperty("RINDEX", energy, rindexAerogel, 2); //adding property to this object

	G4MaterialPropertiesTable *mptQuartz = new G4MaterialPropertiesTable(); //creating aerogel material properties object
	mptQuartz->AddProperty("RINDEX", energy, rindexAerogel, 2); //adding property to this object

	G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable(); //creating properites object for air
	mptWorld->AddProperty("RINDEX", energy, rindexWorld, 2); //adding property to object

	Aerogel->SetMaterialPropertiesTable(mptAerogel); //assigning the Aergoel object the properties from the previous object created

    Quartz->SetMaterialPropertiesTable(mptQuartz); //assigning the Aergoel object the properties from the previous object created

	worldMat->SetMaterialPropertiesTable(mptWorld); //assigning properties object to world material object


//---------------------------------------------------------------------------------

    //Defining Geometries


    //World Volume
    G4double worldX = 0.5*m;
    G4double worldY = 0.5*m;
    G4double worldZ = 0.5*m;

	G4Box *solidWorld = new G4Box("solidWorld", worldX, worldY, worldZ); //setting up world volume as a box

	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld"); //creating the world as dimensions from box and material (Air)

	G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true); //placing the world into geant 4 sim



    //Radiator Volume
    G4double radiatorThickness = 0.002*m;
    G4double radiatorDistance = 0.1*m;
    G4double radiatorSize = 0.4*m;
    G4Material *radiatorMaterial = Aerogel;

	G4Box *solidRadiator = new G4Box("solidRadiator", radiatorSize, radiatorSize, radiatorThickness); //setting the dimensions for the radiator

	G4LogicalVolume *logicRadiator = new G4LogicalVolume(solidRadiator, radiatorMaterial, "logicRadiator"); //creating the aergoel radiator from dimensions

	G4VPhysicalVolume *physRadiator = new G4PVPlacement(0, G4ThreeVector(0., 0., worldZ - radiatorDistance), logicRadiator, "physRadiator", logicWorld, false, 0, true); //placing radiator into world volume



    //Detector Volume (Pixels)
    G4double detectorThickness = 0.01*m;
    G4double detectorSize = 0.005*m;

	G4Box *solidDetector = new G4Box("solidDetector", detectorSize, detectorSize, detectorThickness);  //setting the dimensions for the detectors

	logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector"); //creating the detectors, with same material as world (air)


    //Placing 100X100 pixles/detectors
	for(G4int i = 0; i < 100; i++)
	{

		for(G4int j = 0; j < 100; j++)
		{
            //Pixel/detector positions
		    G4double x_pos = -worldX + (i*m + worldX)/100;
		    G4double y_pos = -worldY + (j*m + worldY)/100;
		    G4double z_pos = worldZ - detectorThickness;
		    G4double index = j+i*100; //unique ID for each pixel/detector

		  G4VPhysicalVolume *physDetector= new G4PVPlacement(0, G4ThreeVector(x_pos, y_pos, z_pos), logicDetector, "physDetector", logicWorld, false, index, true); //placing 100*100 detectors into the world

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

