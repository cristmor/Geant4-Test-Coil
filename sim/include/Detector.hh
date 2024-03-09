#pragma once

#include <math.h> 
#include <iostream>
#include <vector>

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4OpticalSurface.hh"
#include "G4MaterialPropertyVector.hh"
#include "G4RunManager.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4TessellatedSolid.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "CADMesh.hh"

class G4VPhysicalVolume;
class G4GlobalMagFieldMessenger;

class Detector : public G4VUserDetectorConstruction {
public:
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    Detector();
    virtual ~Detector();

    double  getWorldSizeX() {return worldSizeX;}  // half width
    double  getWorldSizeY() {return worldSizeY;}  // half width
    double  getWorldSizeZ() {return worldSizeZ;}  // half width

private:
    static G4ThreadLocal G4GlobalMagFieldMessenger*  fMagFieldMessenger; 

    G4bool  fCheckOverlaps;

    double worldSizeX;
    double worldSizeY;
    double worldSizeZ;

    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();

};

