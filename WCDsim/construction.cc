
#include "construction.hh"
#include "G4LogicalVolume.hh"
#include "G4UserLimits.hh"
#include <vector>

void ReadDataFromFile(const std::string& filename, G4PhysicsFreeVector* vector, G4double unitConversionFactor) {
    std::ifstream datafile(filename);
    if (!datafile.is_open()) {
        G4cerr << "Error: Cannot open " << filename << G4endl;
        return;
    }

    G4double energy, value;
    while (datafile >> energy >> value) {
        vector->InsertValues(energy * eV, value * unitConversionFactor);
    }

    datafile.close();
}

MyDetectorConstruction::MyDetectorConstruction()
{
    xWorld = 0.5*m; //1m
    yWorld = 0.375*m; //0.75m
    zWorld = 0.5*m; //1m

    wlsAbs = new G4PhysicsFreeVector();
    wlsEmm = new G4PhysicsFreeVector();
    wlsRef = new G4PhysicsFreeVector();

    // Load WLS optical spectra from file
    ReadDataFromFile("abslength_kuraray.txt", wlsAbs, cm);   // Absorption length (cm -> mm conversion)
    ReadDataFromFile("emission_kuraray_active.txt", wlsEmm, 1.0);   // Emission spectrum (no unit conversion needed)
    ReadDataFromFile("refractive_index_kuraray.txt", wlsRef, 1.0);  // Refractive index (no unit conversion needed)


    DefineMaterials();
}

MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterials()
{
    G4NistManager *nist = G4NistManager::Instance();  //the geant4 nist material database

//--------------------------ENERGY-and-Optical-Properties---------------------------------

  const G4int nEntries = 99;

  G4double wavelengths[nEntries] = 
  {690, 685, 681, 675, 670, 665, 661, 655, 650, 646, 640, 635, 629, 625, 619,
    615, 610, 605, 600, 596, 590, 585, 579, 575, 570, 565, 560, 555, 550, 545,
    540, 535, 530, 525, 520, 515, 510, 505, 500, 495, 490, 485, 480, 475, 470,
    465, 460, 455, 450, 445, 440, 435, 429, 425, 420, 415, 410, 405, 400, 395,
    390, 385, 380, 375, 370, 365, 360, 355, 350, 345, 340, 335, 330, 325, 320,
    315, 310, 305, 300, 295, 290, 285, 280, 275, 270, 265, 260, 255, 250, 245,
    240, 235, 230, 225, 220, 215, 210, 205, 200};

  G4double photonEnergies[nEntries] = 
  {1.80*eV, 1.81*eV, 1.82*eV, 1.84*eV, 1.85*eV, 1.87*eV, 1.88*eV, 1.90*eV, 1.91*eV, 1.92*eV, 1.94*eV,
    1.96*eV, 1.97*eV, 1.99*eV, 2.01*eV, 2.02*eV, 2.04*eV, 2.05*eV, 2.07*eV, 2.08*eV, 2.10*eV, 2.12*eV,
    2.14*eV, 2.16*eV, 2.18*eV, 2.20*eV, 2.22*eV, 2.24*eV, 2.26*eV, 2.28*eV, 2.30*eV, 2.32*eV, 2.34*eV,
    2.36*eV, 2.39*eV, 2.41*eV, 2.43*eV, 2.46*eV, 2.48*eV, 2.51*eV, 2.53*eV, 2.56*eV, 2.59*eV, 2.61*eV,
    2.64*eV, 2.67*eV, 2.70*eV, 2.73*eV, 2.76*eV, 2.79*eV, 2.82*eV, 2.85*eV, 2.89*eV, 2.92*eV, 2.96*eV,
    2.99*eV, 3.03*eV, 3.07*eV, 3.10*eV, 3.14*eV, 3.18*eV, 3.22*eV, 3.27*eV, 3.31*eV, 3.36*eV, 3.40*eV,
    3.45*eV, 3.50*eV, 3.55*eV, 3.60*eV, 3.65*eV, 3.71*eV, 3.76*eV, 3.82*eV, 3.88*eV, 3.94*eV, 4.00*eV,
    4.07*eV, 4.14*eV, 4.21*eV, 4.28*eV, 4.36*eV, 4.43*eV, 4.51*eV, 4.60*eV, 4.69*eV, 4.78*eV, 4.87*eV,
    4.97*eV, 5.07*eV, 5.17*eV, 5.28*eV, 5.40*eV, 5.52*eV, 5.64*eV, 5.77*eV, 5.91*eV, 6.06*eV, 6.21*eV};

    G4double waterEnergies[9] =
    {1.91*eV, 2.07*eV, 2.25*eV, 2.48*eV, 2.75*eV, 3.09*eV, 3.54*eV, 4.13*eV, 4.50*eV};

    G4double refractiveIndicesWater[9] = 
    {1.33165, 1.33293, 1.33458, 1.33676, 1.3437, 1.34378, 1.34978, 1.35942, 1.37761};
    
	G4double rindexWater[nEntries] =
   {1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33,
   1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33,
   1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33,
   1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33,
   1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33,
   1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33,
   1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33,
   1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33,
   1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33,
   1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33, 1.33};

   G4double waterAbsorptionLengths[nEntries] = 
   {1723.346376, 1837.67053733, 1960.77636972, 2052.38787939, 2115.17723288, 
    2169.92857515, 2256.02471679, 2440.20754551, 2572.55701836, 2663.34346505, 
    2699.09536007, 2711.58018176, 2776.24066169, 2813.49241221, 2932.84008033, 
    3064.11455074, 3261.35401975, 3542.09608102, 4124.19738407, 5109.42794009, 
    6142.47343566, 7419.80568678, 8656.06705003, 9915.75938058, 10999.3128964, 
    11947.8126076, 12925.2985109, 13816.461684, 14906.0435133, 15896.3449567, 
    17034.214619, 18766.9762828, 19970.9227004, 21197.5553858, 21940.0648375, 
    23359.1873386, 26690.6502501, 31013.3005477, 35834.1799397, 41817.9443384, 
    44191.1132346, 44911.1410844, 44807.1913015, 44908.71817, 42692.9941532, 
    41052.8184643, 40120.8791209, 38665.4280273, 36844.5093613, 33910.2167183, 
    30994.8244406, 28273.4868555, 26149.844501, 24060.6989045, 22105.5315081, 
    20530.9655938, 19108.601336, 18000.0, 16765.9214502, 15750.1702868, 
    14666.5257149, 13846.1077844, 12982.1431356, 12230.9760649, 11571.3034654, 
    10899.9345293, 10288.1931999, 9711.91898451, 9168.61803483, 8616.82107551, 
    8133.15095261, 7696.97642164, 7334.81363997, 6987.55980861, 6642.94948528, 
    6271.58893995, 5785.57130942, 5277.21567136, 4789.29865946, 4439.85279215, 
    4002.25010818, 3494.07320349, 3201.76929882, 2893.08726931, 2608.34743373, 
    2357.2848526, 2154.48404379, 1965.12747875, 1779.37668334, 1637.99291617, 
    1514.93775934, 1453.48954968, 1384.42655668, 1286.51706231, 1146.46870868, 
    711.781103035, 361.720766523, 202.448562673, 120.331319206};


    G4double rindexAir[nEntries] =
    {1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003,
   1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003,
   1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003,
   1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003,
   1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003,
   1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003,
   1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003,
   1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003,
   1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003,
   1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003, 1.0003};

   

    
    G4double rindexPoly[nEntries] =
    {1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5,
    1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5,
    1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5,
    1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5,
    1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5,
    1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5,
    1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5,
    1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5,
    1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5,
    1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5};

    G4double rindexGlass[9] =
    {1.480899408, 1.482444444, 1.484429752, 1.48704, 1.490567901, 1.4955, 1.502693878, 1.513777778};

    G4double glassEnergies[9] =
    {1.91*eV, 2.07*eV, 2.25*eV, 2.48*eV, 2.75*eV, 3.09*eV, 3.54*eV, 4.13*eV, 4.50*eV};

    G4double glassAbsEnergies[12] = 
    {1.91*eV, 2.57029*eV, 2.67224*eV, 2.90252*eV, 3.12969*eV, 3.39928*eV, 3.6385*eV, 3.9134*eV, 4.0485*eV, 4.11735*eV, 4.1370*eV, 4.50*eV};     

    G4double glassAbsLen[12] =
    {9.354*mm, 9.354228583*mm, 9.340137205*mm, 9.310446926*mm, 7.945329114*mm, 5.989758165*mm, 4.479283738*mm, 2.724705049*mm, 1.822596084*mm,
     1.108868073*mm, 0.832238546*mm, 0.188*mm};

//--------------------------PMMA-WLS---------------------------------

  PMMA = new G4Material("PMMA", 1.190*g/cm3, 3); //defining material object
  PMMA->AddElement(nist->FindOrBuildElement("O"), 2);
  PMMA->AddElement(nist->FindOrBuildElement("H"), 8); //adding the hydrogen element to object
  PMMA->AddElement(nist->FindOrBuildElement("C"), 5); //adding the Carbon element to object

//--------------------------WLS-PROPERTIES---------------------------------

	G4MaterialPropertiesTable *mptWLS = new G4MaterialPropertiesTable();
	mptWLS->AddProperty("RINDEX", wlsRef); //adding property to this object
	mptWLS->AddProperty("WLSABSLENGTH", wlsAbs);
    mptWLS->AddProperty("WLSCOMPONENT", wlsEmm);
    mptWLS->AddConstProperty("WLSTIMECONSTANT", 3*ns);

	PMMA->SetMaterialPropertiesTable(mptWLS);             

//--------------------------POLYETHYLENE---------------------------------


    Polyethylene = new G4Material("Polyethylene", 0.96*g/cm3, 2); //defining material object Tyvek
    Polyethylene->AddElement(nist->FindOrBuildElement("H"), 2); //adding the hydrogen element to object
    Polyethylene->AddElement(nist->FindOrBuildElement("C"), 1); //adding the Carbon element to object

    G4MaterialPropertiesTable *mptPoly= new G4MaterialPropertiesTable(); //creating properites object for air
    mptPoly->AddProperty("RINDEX", photonEnergies, rindexPoly, nEntries); //adding property to object

    Polyethylene->SetMaterialPropertiesTable(mptPoly); //assigning properties object to world material object
    

//--------------------------WATER---------------------------------

	//Water = nist->FindOrBuildMaterial("G4_WATER"); //creating material
    Water = new G4Material("Water", 1*g/cm3, 2); //defining material object
    Water->AddElement(nist->FindOrBuildElement("H"), 2);
    Water->AddElement(nist->FindOrBuildElement("O"), 1);

	G4MaterialPropertiesTable *mptWater= new G4MaterialPropertiesTable(); //creating properites object for air
	mptWater->AddProperty("RINDEX", waterEnergies, refractiveIndicesWater, 9); //adding property to object
    mptWater->AddProperty("ABSLENGTH", photonEnergies, waterAbsorptionLengths, nEntries); //adding property to object


	Water->SetMaterialPropertiesTable(mptWater); //assigning properties object to world material object

//--------------------------AIR---------------------------------

    Air = nist->FindOrBuildMaterial("G4_AIR");

    G4MaterialPropertiesTable *mptAir = new G4MaterialPropertiesTable();
    mptAir->AddProperty("RINDEX", photonEnergies, rindexAir, nEntries);

    Air->SetMaterialPropertiesTable(mptAir);


//--------------------------GLASS-------------------------------

    G4Material* SiO2 = new G4Material("SiO2", 2.23*g/cm3, 2);
    SiO2->AddElement(nist->FindOrBuildElement("Si"), 1);
    SiO2->AddElement(nist->FindOrBuildElement("O") , 2);

    G4Material* B2O2 = new G4Material("B2O2", 2.23*g/cm3, 2);
    B2O2->AddElement(nist->FindOrBuildElement("B"), 2);
    B2O2->AddElement(nist->FindOrBuildElement("O") , 2);

    G4Material* Na2O = new G4Material("Na2O", 2.23*g/cm3, 2);
    Na2O->AddElement(nist->FindOrBuildElement("Na"), 2);
    Na2O->AddElement(nist->FindOrBuildElement("O") , 1);

    Glass = new G4Material("Glass", 2.23*g/cm3,3);
    Glass->AddMaterial(SiO2, .80);
    Glass->AddMaterial(B2O2, .13);
    Glass->AddMaterial(Na2O, .07);

    G4MaterialPropertiesTable *mptGlass= new G4MaterialPropertiesTable(); //creating properites object for air
    mptGlass->AddProperty("RINDEX", glassEnergies, rindexGlass, 9); //adding property to object
    mptGlass->AddProperty("ABSLENGTH", glassAbsEnergies, glassAbsLen, 12); //adding property to object


    Glass->SetMaterialPropertiesTable(mptGlass); //assigning properties object to world material object


//--------------------------MIRROR-PROPERTIES---------------------------------

	mirrorSurface = new G4OpticalSurface("mirrorSurface");

    mirrorSurface->SetType(dielectric_metal);
    //mirrorSurface->SetFinish(groundfrontpainted);
    mirrorSurface->SetFinish(polished);
    mirrorSurface->SetModel(unified);

    //G4double reflectivity[9] =
    //{0.95, 0.95, 0.95, 0.95, 0.95, 0.95, 0.95, 0.95, 0.95};
    G4double reflectivity[9] =
    {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

    G4double MirrorEnergies[9] = 
    {1.907, 2.066, 2.254, 2.479, 2.755, 3.099, 3.542, 4.132, 4.959};

    G4double specularLobeConstant[2] ={0.2, 0.2};
    G4double specularLobeConstantEnergies[2] ={1.907, 4.959};


    G4MaterialPropertiesTable *mptMirror = new G4MaterialPropertiesTable();
    mptMirror->AddProperty("REFLECTIVITY", MirrorEnergies, reflectivity, 9);
    //mptMirror->AddProperty("SPECULARLOBECONSTANT", specularLobeConstantEnergies, specularLobeConstant, 2);

    mirrorSurface->SetMaterialPropertiesTable(mptMirror);

}

void MyDetectorConstruction::ConstructWLS()
{
    // Use half measurements as Geant4 doubles them

    //0.002 -> 4mm '
    //0.0025 -> 5mm '
    //0.003 -> 6mm '
    //0.0035 -> 7mm '
    //0.004 -> 8mm '
    //0.0045 -> 9mm '
    //0.005 -> 10mm '
    //0.0055 -> 11mm '
    G4double WLS_thickness = 0.0045*m;
    //0.15 -> 30cm '
    //0.175 -> 35cm '
    //0.2 -> 40cm '
    //0.225 -> 45cm '
    //0.25 -> 50cm '
    //0.275 -> 55cm '
    //0.3 -> 60cm '
    G4double WLS_size = 0.225*m;

    G4double WLS_Mirror_thickness = 0.0005*m; //2mm
    G4double WLS_Mirror_box_width = WLS_thickness; //10mm
    G4double WLS_Mirror_size = WLS_size + WLS_Mirror_thickness; //15.05cm

    G4double PMT_radius = 0.0381*m; //3-inches diameter in metres
    //G4double PMT_radius = 0.1016*m; //8-inches diameter in metres

	G4RotationMatrix* Rotation = new G4RotationMatrix();
	Rotation->rotateX(270*deg); //rotate to face downwards

//--------------------------PMT---------------------------------

//SPHERE

	solidPMTSphere = new G4Sphere("solidPMTSphere", 0., PMT_radius, 0., 2*pi, 0., 0.5*pi);  //setting the dimensions for the detectors

	logicPMTSphere = new G4LogicalVolume(solidPMTSphere, Glass, "logicPMTSphere"); 


//--------------------------WLS---------------------------------

	solidWLS = new G4Box("solidWLSBox", WLS_size, WLS_thickness, WLS_size);

    //Remove the hemisphere (PMT) from the bottom of the WLS box
    G4VSolid* WLS = new G4SubtractionSolid("WLS-PMTCylinder", solidWLS, solidPMTSphere, Rotation, G4ThreeVector(0., WLS_thickness, 0.));

    logicWLS = new G4LogicalVolume(WLS, PMMA, "logicWLS");

    fScoringVolume = logicWLS;


       
//WLS Mirror

    solidMirrorWLSBox = new G4Box("solidMirrorTankBox", WLS_Mirror_size ,WLS_Mirror_box_width, WLS_Mirror_size);

    //Remove WLS from WLS mirror volume
	G4VSolid* WLSMirrorSubtract    = new G4SubtractionSolid("mirrorWLSSubtract", solidMirrorWLSBox, solidWLS, 0, G4ThreeVector(0., 0., 0.));

	logicWLSMirror = new G4LogicalVolume(WLSMirrorSubtract, PMMA, "logicWLSMirror"); //creating the detectors, with same material as world

    G4LogicalSkinSurface *WLSMirrorSkin = new G4LogicalSkinSurface("WLSMirrorSkin", logicWLSMirror, mirrorSurface); //apply reflective skin surface to mirror volume

//--------------------------LINER-VOLUME---------------------------------
    G4Box *solidLiner = new G4Box("solidLiner", xWorld, yWorld, zWorld); //setting up world volume as a box

	G4LogicalVolume *logicLiner = new G4LogicalVolume(solidLiner, Water, "logicLiner"); //creating the world as dimensions from box and material

	G4PVPlacement *physLiner = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicLiner, "physLiner", logicWorld, false, 7, true); //placing the world into geant 4 sim

    G4LogicalSkinSurface *MirrorSkin = new G4LogicalSkinSurface("worldMirrorSkin", logicLiner, mirrorSurface); //apply reflective skin surface to mirror volume
    

//--------------------------WATER-VOLUME---------------------------------
    G4Box *solidWater = new G4Box("solidWater", xWorld-1*mm, yWorld-1*mm, zWorld-1*mm); //setting up world volume as a box

	G4LogicalVolume *logicWater = new G4LogicalVolume(solidWater, Water, "logicWater"); //creating the world as dimensions from box and material

	G4PVPlacement *physWater = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWater, "physWater", logicLiner, false, 8, true); //placing the world into geant 4 sim

    //Placing after mother volumes
    physPMTSphere = new G4PVPlacement(Rotation, G4ThreeVector(0., yWorld-1*mm, 0.), logicPMTSphere, "physPMTSphere", logicWater, false, 0, true); //place PMT at top of tank, facing downwards
    physWLS = new G4PVPlacement(0, G4ThreeVector(0., yWorld-1*mm - WLS_thickness, 0.), logicWLS, "physWLS", logicWater, false, 1, true); //placing WLS into world volume, at top of tank, facing down
    physWLSMirror = new G4PVPlacement(0, G4ThreeVector(0., yWorld-1*mm - WLS_Mirror_box_width, 0.), logicWLSMirror, "physWLSMirror", logicWater, false, 2, true); // Place in center of tank, to line inner edges of tank

}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{

	G4RotationMatrix* Rotation = new G4RotationMatrix();
    Rotation->rotateX(270*deg); //rotate to face downwards

    //--------------------------WORLD-VOLUME---------------------------------

    solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld); //setting up world volume as a box


	logicWorld = new G4LogicalVolume(solidWorld, Air, "logicWorld"); //creating the world as dimensions from box and material

	physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 6, true); //placing the world into geant 4 sim

    ConstructWLS();

	return physWorld; //returning the world, including everything inside
}

void MyDetectorConstruction::ConstructSDandField()
{

//--------------------------SENSITIVE-DETECORS---------------------------------
    MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector"); //creating a sensitive detector object

    if(logicPMTSphere != NULL)
        logicPMTSphere->SetSensitiveDetector(sensDet); //assigning the detectors as senstive detectors (it generates a hit when a track traverses its volume)

}
