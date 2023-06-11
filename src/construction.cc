#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{}

MyDetectorConstruction::~MyDetectorConstruction()
{}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    G4NistManager *nist = G4NistManager::Instance();

    // *********************************
    //              MATERIALS
    // *********************************

    // WORLD
    //
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");

    //conversion factor [ev] / wavelength [mu_m] (blue 200 nm, red 900 nm) 
    G4double energy[2] = {1.239841939*eV/0.9, 1.239841939*eV/0.2};
    //refractive index for world
    G4double rindexWorld[2] = {1.0, 1.0};

    G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
    mptWorld->AddProperty("RINDEX", energy, rindexWorld, 2);

    worldMat->SetMaterialPropertiesTable(mptWorld);
    
    // AEROGEL
    //
    G4Material *Si02 = new G4Material("Si02", 2.201*g/cm3, 2);
    Si02->AddElement(nist->FindOrBuildElement("Si"), 1);
    Si02->AddElement(nist->FindOrBuildElement("O"), 2);

    G4Material *H20 = new G4Material("H20", 1.000*g/cm3, 2);
    H20->AddElement(nist->FindOrBuildElement("H"), 2);
    H20->AddElement(nist->FindOrBuildElement("O"), 1);

    G4Element *C= nist->FindOrBuildElement("C");

    G4Material *Aerogel = new G4Material("Aerogel", 0.200*g/cm3, 3);
    Aerogel->AddMaterial(Si02, 62.5*perCent);
    Aerogel->AddMaterial(Si02, 62.5*perCent);
    Aerogel->AddElement(C, 0.1*perCent);

    //refractive index for aerogel
    G4double rindexAerogel[2] = {1.1, 1.1};

    G4MaterialPropertiesTable *mptAerogel = new G4MaterialPropertiesTable();
    mptAerogel->AddProperty("RINDEX", energy, rindexAerogel, 2);

    Aerogel->SetMaterialPropertiesTable(mptAerogel);



    // *********************************
    //              GEOMETRY
    // *********************************

    // WORLD
    //
    G4Box *solidWorld = new G4Box("solidWorld", 0.5*m, 0.5*m, 0.5*m);

    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, 
                                                    "logicWorld");

    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), 
                                                    logicWorld, "physWorld", 0,
                                                    false, 0, true);


    // AEROGEL
    //
    G4Box *solidRadiator = new G4Box("solidRadiator", 0.4*m, 0.4*m, 0.01*m);

    G4LogicalVolume *logicRadiator = new G4LogicalVolume(solidRadiator, Aerogel, "logicRadiator");

    G4VPhysicalVolume *physRadiator = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.25*m), 
                                                        logicRadiator, "physRadiator", logicWorld, 
                                                        false, 0, true);

    // DETECTOR
    //
    G4Box *solidDetector = new G4Box("solidDetector", 0.005*m, 0.005*m, 0.01*m);

    logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");

    // create an array of detectors
    for(G4int i = 0; i < 100; i++)
    {
        for(G4int j = 0; j < 100; j++)
        {
            G4VPhysicalVolume *physDetector = new G4PVPlacement(0, G4ThreeVector(-0.5*m+(i+0.5)*m/100, -0.5*m+(j+0.5)*m/100, 0.49*m),
                                                                logicDetector, "physDetector", logicWorld, false, j+i*100, true);
        }
    }

    return physWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{
    MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");

    logicDetector->SetSensitiveDetector(sensDet);
}