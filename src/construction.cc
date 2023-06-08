#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{}

MyDetectorConstruction::~MyDetectorConstruction()
{}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    G4NistManager *nist = G4NistManager::Instance();

    //
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

    //conversion factor [ev] / wavelength [mu_m] (blue 200 nm, red 900 nm) 
    G4double energy[2] = {1.239841939*eV/0.2, 1.239841939*eV/0.9};
    //refractive index for aerogel
    G4double rindexAerogel[2] = {1.1, 1.1};

    G4MaterialPropertiesTable *mptAerogel = new G4MaterialPropertiesTable();
    mptAerogel->AddProperty("RINDEX", energy, rindexAerogel, 2);

    Aerogel->SetMaterialPropertiesTable(mptAerogel);

    //
    // WORLD
    //
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");

    //refractive index for world
    G4double rindexWorld[2] = {1.0, 1.0};

    G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
    mptWorld->AddProperty("RINDEX", energy, rindexWorld, 2);

    worldMat->SetMaterialPropertiesTable(mptWorld);

    //
    // GEOMETRY
    //
    G4Box *solidWorld = new G4Box("solidWorld", 0.5*m, 0.5*m, 0.5*m);

    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, 
                                                    "logicWorld");

    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), 
                                                    logicWorld, "physWorld", 0,
                                                    false, 0, true);

    G4Box *solidRadiator = new G4Box("solidRadiator", 0.4*m, 0.4*m, 0.01*m);

    G4LogicalVolume *logicRadiator = new G4LogicalVolume(solidRadiator, Aerogel, "logicRadiator");

    G4VPhysicalVolume * physRadiator = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.25*m), 
                                                        logicRadiator, "physRadiator", logicWorld, 
                                                        false, 0, true);

    return physWorld;
}