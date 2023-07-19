// make sure not included several times (if not define)
#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"

#include "detector.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    MyDetectorConstruction();
    ~MyDetectorConstruction();

    G4LogicalVolume *GetScoringVolume() const { return fScoringVolume; }

    virtual G4VPhysicalVolume *Construct();

  private:
    //Create the geometry
    G4Box *solidWorld, *solidRadiator, *solidDetector;
    G4LogicalVolume *logicWorld, *logicRadiator, *logicDetector;
    G4VPhysicalVolume *physWorld, *physRadiator, *physDetector;

    G4Material *worldMat, *Si02, *H20, *Aerogel;
    G4Element *C;

    virtual void ConstructSDandField();
    void DefineMaterials();

    G4GenericMessenger *fMessenger;
    G4int nCols, nRows;

    G4LogicalVolume *fScoringVolume;

};

#endif