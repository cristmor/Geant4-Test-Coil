
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: DetectorConstruction.cc 101905 2016-12-07 11:34:39Z gunter $
// 
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
#include <math.h> 
#include <iostream>
#include <vector>
#include "DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Sphere.hh" // included by rp for sphere
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

//#define USE_CADMESH_TETGEN

//#define USE_CADMESH_TETGEN
#include "CADMesh.hh"
//#include "tetgen.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal 
G4GlobalMagFieldMessenger* DetectorConstruction::fMagFieldMessenger = nullptr; 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction(CoilTree* coilTree)
 : G4VUserDetectorConstruction(),
   fCoilTree(coilTree),
   fCheckOverlaps(true)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Define materials 
  DefineMaterials();
  
  // Define volumes
  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{ 
  // Some materials are defined using NIST Manager
  auto nistManager = G4NistManager::Instance();
  
  G4double density;
  G4int ncomponents, natoms; 
  G4Element* C  = nistManager->FindOrBuildElement(6);
  G4Element* H  = nistManager->FindOrBuildElement(1);
  G4Element* O  = nistManager->FindOrBuildElement(8);
  G4Element* F  = nistManager->FindOrBuildElement(9);

  G4Material* Scintillator = // Scintillator material
	new G4Material("Scintillator", density= 1.032*g/cm3, ncomponents=2);
	Scintillator->AddElement(C, natoms=9);
	Scintillator->AddElement(H, natoms=10);
	Scintillator->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

  nistManager->FindOrBuildMaterial("G4_AIR"); // Air material
  nistManager->FindOrBuildMaterial("G4_Cu"); // Copper material


    ///--- for scintillation fiber core ---
  G4Material* polystyrene =
  new G4Material("Polystyrene",density= 1.05*g/cm3, ncomponents=2);
  polystyrene->AddElement(C, natoms=8);
  polystyrene->AddElement(H, natoms=8);

  ///--- for cladding (scintillation fibers) ---
  G4Material* pmma_clad =
  new G4Material("PMMA_Clad",density= 1.19*g/cm3, ncomponents=3);
  pmma_clad->AddElement(C, natoms=5);
  pmma_clad->AddElement(H, natoms=8);
  pmma_clad->AddElement(O, natoms=2);

  ///--- for Cerenkov fiber core ---
  G4Material* pmma =
  new G4Material("PMMA",density= 1.19*g/cm3, ncomponents=3);
  pmma->AddElement(C, natoms=5);
  pmma->AddElement(H, natoms=8);
  pmma->AddElement(O, natoms=2);

  ///--- for cladding (Cerenkov fibers) ---
  G4Material* fluorinatedPolymer =
  new G4Material("Fluorinated_Polymer", density= 1.43*g/cm3, ncomponents=2);
  fluorinatedPolymer->AddElement(C,2);
  fluorinatedPolymer->AddElement(F,2);
  G4MaterialPropertiesTable* mpPMMA;
  G4MaterialPropertiesTable* mpFS;

  //--- Generate and add material properties table ---
  G4double PhotonEnergy[] = {2.00*eV,2.03*eV,2.06*eV,2.09*eV,2.12*eV,
      2.15*eV,2.18*eV,2.21*eV,2.24*eV,2.27*eV,
      2.30*eV,2.33*eV,2.36*eV,2.39*eV,2.42*eV,
      2.45*eV,2.48*eV,2.51*eV,2.54*eV,2.57*eV,
      2.60*eV,2.63*eV,2.66*eV,2.69*eV,2.72*eV,
      2.75*eV,2.78*eV,2.81*eV,2.84*eV,2.87*eV,
      2.90*eV,2.93*eV,2.96*eV,2.99*eV,3.02*eV,
      3.05*eV,3.08*eV,3.11*eV,3.14*eV,3.17*eV,
      3.20*eV,3.23*eV,3.26*eV,3.29*eV,3.32*eV,
      3.35*eV,3.38*eV,3.41*eV,3.44*eV,3.47*eV};

  const G4int nEntries = sizeof(PhotonEnergy) / sizeof(G4double);

  //--- PMMA ---
  G4double pmma_Index = 1.49;
  G4double RefractiveIndex_PMMA[nEntries] =
  {       
          pmma_Index, pmma_Index, pmma_Index, pmma_Index, pmma_Index, pmma_Index, pmma_Index, pmma_Index, pmma_Index, pmma_Index,
          pmma_Index, pmma_Index, pmma_Index, pmma_Index, pmma_Index, pmma_Index, pmma_Index, pmma_Index, pmma_Index, pmma_Index,
          pmma_Index, pmma_Index, pmma_Index, pmma_Index, pmma_Index, pmma_Index, pmma_Index, pmma_Index, pmma_Index, pmma_Index,
          pmma_Index, pmma_Index, pmma_Index, pmma_Index, pmma_Index, pmma_Index, pmma_Index, pmma_Index, pmma_Index, pmma_Index,
          pmma_Index, pmma_Index, pmma_Index, pmma_Index, pmma_Index, pmma_Index, pmma_Index, pmma_Index, pmma_Index, pmma_Index
  };
  mpPMMA = new G4MaterialPropertiesTable();
  mpPMMA->AddProperty("RINDEX",PhotonEnergy,RefractiveIndex_PMMA,nEntries);
  pmma->SetMaterialPropertiesTable(mpPMMA);

  //--- Fluorinated Polymer (FS) ---
  G4double fp_Index = 1.30;
  G4double RefractiveIndex_FluorinatedPolymer[nEntries] =
  {       
          fp_Index, fp_Index, fp_Index, fp_Index, fp_Index, fp_Index, fp_Index, fp_Index, fp_Index, fp_Index,
          fp_Index, fp_Index, fp_Index, fp_Index, fp_Index, fp_Index, fp_Index, fp_Index, fp_Index, fp_Index,
          fp_Index, fp_Index, fp_Index, fp_Index, fp_Index, fp_Index, fp_Index, fp_Index, fp_Index, fp_Index,
          fp_Index, fp_Index, fp_Index, fp_Index, fp_Index, fp_Index, fp_Index, fp_Index, fp_Index, fp_Index,
          fp_Index, fp_Index, fp_Index, fp_Index, fp_Index, fp_Index, fp_Index, fp_Index, fp_Index, fp_Index
  };
  mpFS = new G4MaterialPropertiesTable();
  mpFS->AddProperty("RINDEX",PhotonEnergy,RefractiveIndex_FluorinatedPolymer,nEntries);
  fluorinatedPolymer->SetMaterialPropertiesTable(mpFS);



  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
  /*  G4 Gerometry Tree
     World
       - Copper Block
         - G4Coil
         - Fiber
  */

  // Get materials
  auto defaultMaterial = G4Material::GetMaterial("G4_AIR");
  auto cladMaterial = G4Material::GetMaterial("Fluorinated_Polymer");
  auto coreMaterial = G4Material::GetMaterial("PMMA");
  auto CopperMaterial = G4Material::GetMaterial("G4_Cu");

  if ( ! defaultMaterial || ! cladMaterial || ! coreMaterial || ! CopperMaterial ) {
    G4ExceptionDescription msg;
    msg << "Cannot retrieve materials already defined."; 
    G4Exception("DetectorConstruction::DefineVolumes()",
      "MyCode0001", FatalException, msg);
  }

  //     
  // World
  //

  worldSizeX = 10.0*cm ;  // half width
  worldSizeY = 10.0*cm ;  // half width
  worldSizeZ = 20.0*cm ;  // half width

  auto worldS = new G4Box("World", worldSizeX/2, worldSizeY/2, worldSizeZ/2);
  auto worldLV = new G4LogicalVolume(worldS, defaultMaterial, "World");
  auto worldPV = new G4PVPlacement(0, G4ThreeVector(), worldLV, "World", 0, false, 0, fCheckOverlaps);

  //
  // Copper Block
  //

  G4double copperBlockX = worldSizeX - 5.0*cm;
  G4double copperBlockY = worldSizeY - 5.0*cm;
  G4double copperBlockZ = worldSizeZ - 5.0*cm;

  auto copperBlockS = new G4Box("copperBlockS", copperBlockX/2, copperBlockY/2, copperBlockZ/2);
  auto copperBlockLV = new G4LogicalVolume(copperBlockS, CopperMaterial, "copperBlockLV");
  auto copperBlockPV = new G4PVPlacement(0, G4ThreeVector(0.0*m, 0.0*m, 0.0*m), copperBlockLV, "copperBlockPV", worldLV, false, 0, fCheckOverlaps);

  //
	// G4Coil
	//


	//
	// Fiber
	//

	G4double CladRadius = 0.5*mm;
	G4double CoreRadius = 0.45*mm;
	G4double height = copperBlockZ - 2.0*cm;

	// Create a G4Tubs object with the specified dimensions
  // auto FiberClad = CADMesh::TessellatedMesh::FromSTL("G4FiberClad.stl");
  // FiberClad->SetScale(1.0);
  auto fiberCladS = new G4Tubs("fiberCladS", 0.0, CladRadius, height/2, 0.0, 2.0*M_PI);
	auto fiberCladLV = new G4LogicalVolume( fiberCladS, cladMaterial, "fiberCladLV");
  auto fiberCladPV = new G4PVPlacement(0, G4ThreeVector(0.0*m, 0.0*m, 0.0*m), fiberCladLV, "fiberCladPV", copperBlockLV, false, 0, fCheckOverlaps);

  // auto FiberCoreS = FiberCore->GetSolid();
  // // G4Tubs* FiberCoreS = new G4Tubs("FiberCoreS", 0.0, CoreRadius, height/2, 0.0, 2.0*M_PI);
  // auto FiberCoreLV = new G4LogicalVolume( FiberCoreS, coreMaterial, "FiberCoreLV");
  // auto FiberCorePV = new G4PVPlacement(0, G4ThreeVector(0.0*mm, 0.0*mm, 0.0*mm), FiberCoreLV, "FiberCorePV", FiberCladLV, false, 0, fCheckOverlaps);

	//                                       
	// Visualization attributes
	//

	worldLV->SetVisAttributes(new G4VisAttributes(TRUE,G4Colour(0.0,0.0,1.0)));
  copperBlockLV->SetVisAttributes(new G4VisAttributes(TRUE,G4Colour(0.0,1.0,0.0)));
  // G4CoilCladLV->SetVisAttributes(new G4VisAttributes(TRUE,G4Colour(0.0,1.0,0.0)));
  // G4CoilCoreLV->SetVisAttributes(new G4VisAttributes(TRUE,G4Colour(0.0,0.0,1.0)));
	fiberCladLV->SetVisAttributes(new G4VisAttributes(TRUE,G4Colour(0.0,1.0,0.0)));
  // FiberCoreLV->SetVisAttributes(new G4VisAttributes(TRUE,G4Colour(0.0,0.0,1.0)));

	//
	// Always return the physical World
	//
	return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{ 
  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
  G4ThreeVector fieldValue;
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);
  
  // Register the field messenger for deleting
  G4AutoDelete::Register(fMagFieldMessenger);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......