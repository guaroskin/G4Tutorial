#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
    fMessenger = new G4GenericMessenger(this, "/detector/", "Detector Construction");

    fMessenger->DeclareProperty("nCols", nCols, "Number of columns");
    fMessenger->DeclareProperty("nRows", nRows, "Number of rows");
    fMessenger->DeclareProperty("isCherenkov", isCherenkov, "Toggle Cherenkov setup");
    fMessenger->DeclareProperty("isScintillator", isScintillator, "Toggle Cherenkov setup");

    nCols = 100;
    nRows = 100;

    DefineMaterials();

    xWorld = 0.5*m;
    yWorld = 0.5*m;
    zWorld = 0.5*m;

    isCherenkov = false;
    isScintillator = true;
}

MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterials()
{
    G4NistManager *nist = G4NistManager::Instance();

    // AIR (World Mat)
    //
    worldMat = nist->FindOrBuildMaterial("G4_AIR");

    //conversion factor [ev] / wavelength [mu_m] (blue 900 nm, red 200 nm) 
    G4double energy[2] = {1.239841939*eV/0.9, 1.239841939*eV/0.2};

    //refractive index for AIR
    //
    G4double rindexWorld[2] = {1.0, 1.0};

    G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
    mptWorld->AddProperty("RINDEX", energy, rindexWorld, 2);

    worldMat->SetMaterialPropertiesTable(mptWorld);
    
    // AEROGEL (Radiator and Detector Mat)
    //
    Si02 = new G4Material("Si02", 2.201*g/cm3, 2);
    Si02->AddElement(nist->FindOrBuildElement("Si"), 1);
    Si02->AddElement(nist->FindOrBuildElement("O"), 2);

    H20 = new G4Material("H20", 1.000*g/cm3, 2);
    H20->AddElement(nist->FindOrBuildElement("H"), 2);
    H20->AddElement(nist->FindOrBuildElement("O"), 1);

    C = nist->FindOrBuildElement("C");

    Aerogel = new G4Material("Aerogel", 0.200*g/cm3, 3);
    Aerogel->AddMaterial(Si02, 62.5*perCent);
    Aerogel->AddMaterial(Si02, 62.5*perCent);
    Aerogel->AddElement(C, 0.1*perCent);

    //refractive index for AEROGEL
    //
    G4double rindexAerogel[2] = {1.1, 1.1};

    G4MaterialPropertiesTable *mptAerogel = new G4MaterialPropertiesTable();
    mptAerogel->AddProperty("RINDEX", energy, rindexAerogel, 2);

    Aerogel->SetMaterialPropertiesTable(mptAerogel);

    // SCINTILLATOR
    //
    NaI = new G4Material("NaI", 3.67*g/cm3, 2);
    Na = nist->FindOrBuildElement("Na");
    I = nist->FindOrBuildElement("I");
    NaI->AddElement(Na, 1);
    NaI->AddElement(I, 1);
    

}

void MyDetectorConstruction::ConstructCherenkov()
{
    // RADIATOR
    //
    solidRadiator = new G4Box("solidRadiator", 0.4*m, 0.4*m, 0.01*m);

    logicRadiator = new G4LogicalVolume(solidRadiator, Aerogel, "logicRadiator");
    //fScoringVolume = logicRadiator;
    
    physRadiator = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.25*m), 
                    logicRadiator, "physRadiator", logicWorld, false, 0, true);

    // DETECTOR
    //
    solidDetector = new G4Box("solidDetector", xWorld/nRows, yWorld/nCols, 0.01*m);

    logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");

    // create an array of detectors
    for(G4int i = 0; i < nRows; i++)
    {
        for(G4int j = 0; j < nCols; j++)
        {
            physDetector = new G4PVPlacement(0, G4ThreeVector(-0.5*m+(i+0.5)*m/nRows, 
                -0.5*m+(j+0.5)*m/nCols, 0.49*m),logicDetector, "physDetector", 
                logicWorld, false, j+i*nCols, true);
        }
    }
}

void MyDetectorConstruction::ConstructScintillator()
{
    solidScintillator = new G4Tubs("solidScintillator", 10*cm, 20*cm, 30*cm,
                                    0*deg, 360*deg);

    logicScintillator = new G4LogicalVolume(solidScintillator, NaI, "logicScintillator");

    fScoringVolume = logicScintillator;

    physScintillator = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), 
                    logicScintillator, "physScintillator", logicWorld, false, 0, true);

}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{

    // WORLD
    //

    solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);

    logicWorld = new G4LogicalVolume(solidWorld, worldMat, 
                                                    "logicWorld");

    physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 
                                                0, false, 0, true);
    
    if(isCherenkov)
        ConstructCherenkov();

    if(isScintillator)
        ConstructScintillator();

    return physWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{
    MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");

    if(isCherenkov)
        logicDetector->SetSensitiveDetector(sensDet);
}