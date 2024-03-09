#include "Detector.hh"

G4ThreadLocal
G4GlobalMagFieldMessenger* Detector::fMagFieldMessenger = nullptr; 

Detector::Detector()
    : G4VUserDetectorConstruction(), fCheckOverlaps(true) {
}
Detector::~Detector() {
}

G4VPhysicalVolume* Detector::Construct(){ 
    DefineMaterials();
    return DefineVolumes();
}

void Detector::DefineMaterials() { 
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

G4VPhysicalVolume* Detector::DefineVolumes()
{
    /*  G4 Gerometry Tree
        World
        - Copper Block
            - Coil
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
            G4Exception("Detector::DefineVolumes()",
            "MyCode0001", FatalException, msg);
    }

    //     
    // World
    //

    worldSizeX = 2.0*cm ;  // half width
    worldSizeY = 2.0*cm ;  // half width
    worldSizeZ = 2.0*cm ;  // half width

    auto worldS = new G4Box("World", worldSizeX/2, worldSizeY/2, worldSizeZ/2);
    auto worldLV = new G4LogicalVolume(worldS, defaultMaterial, "World");
    auto worldPV = new G4PVPlacement(0, G4ThreeVector(), worldLV, "World", 0, false, 0, fCheckOverlaps);

    //
    // Copper Block
    //

    G4double copperBlockX = worldSizeX - 1.0*mm;
    G4double copperBlockY = worldSizeY - 1.0*mm;
    G4double copperBlockZ = worldSizeZ - 1.0*mm;

    auto copperBlockS = new G4Box("copperBlockS", copperBlockX/2, copperBlockY/2, copperBlockZ/2);
    auto copperBlockLV = new G4LogicalVolume(copperBlockS, CopperMaterial, "copperBlockLV");
    new G4PVPlacement(0, G4ThreeVector(0.0*m, 0.0*m, 0.0*m), copperBlockLV, "copperBlockPV", worldLV, false, 0, fCheckOverlaps);

    //
    // G4Coil
    //

    auto coilClad = CADMesh::TessellatedMesh::FromSTL("clad.stl");
    coilClad->SetScale(1000.0);
    auto coilCladS = coilClad->GetSolid();
    auto coilCladLV = new G4LogicalVolume(coilCladS, cladMaterial, "coilCladLV");
    new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 0.0), coilCladLV, "coilCladPV", copperBlockLV, false, 0, fCheckOverlaps);

    auto coilcore = CADMesh::TessellatedMesh::FromSTL("core.stl");
    coilcore->SetScale(1000.0);
    auto coilCoreS = coilcore->GetSolid();
    auto coilCoreLV = new G4LogicalVolume(coilCoreS, coreMaterial, "coilCladLV");
    new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 0.0), coilCoreLV, "coilCladPV", coilCladLV, false, 0, fCheckOverlaps);

    //
    // Fiber
    //

    G4double cladRadius = 0.5*mm;
    G4double coreRadius = 0.45*mm;
    G4double height = copperBlockZ;

    auto fiberCladS = new G4Tubs("fiberCladS", 0.0, cladRadius, height/2, 0.0, 2.0*M_PI);
    auto fiberCladLV = new G4LogicalVolume(fiberCladS, cladMaterial, "fiberCladLV");
    new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 0.0), fiberCladLV, "fiberCladPV", copperBlockLV, false, 0, fCheckOverlaps);

    auto fiberCoreS = new G4Tubs("fiberCoreS", 0.0, coreRadius, height/2, 0.0, 2.0*M_PI);
    auto fiberCoreLV = new G4LogicalVolume( fiberCoreS, coreMaterial, "fiberCoreLV");
    new G4PVPlacement(0, G4ThreeVector(0.0*mm, 0.0*mm, 0.0*mm), fiberCoreLV, "fiberCorePV", fiberCladLV, false, 0, fCheckOverlaps);

    //                                       
    // Visualization attributes
    //

    worldLV->SetVisAttributes(new G4VisAttributes(TRUE,G4Colour(0.0,0.0,1.0)));
    copperBlockLV->SetVisAttributes(new G4VisAttributes(TRUE,G4Colour(0.0,1.0,0.0)));
    coilCladLV->SetVisAttributes(new G4VisAttributes(TRUE,G4Colour(0.5, 0.0, 0.5)));
    coilCoreLV->SetVisAttributes(new G4VisAttributes(TRUE,G4Colour(0.0,0.0,1.0)));
    fiberCladLV->SetVisAttributes(new G4VisAttributes(TRUE,G4Colour(0.5, 0.0, 0.5)));
    fiberCoreLV->SetVisAttributes(new G4VisAttributes(TRUE,G4Colour(0.0,0.0,1.0)));

    //
    // Always return the physical World
    //
    return worldPV;
}

void Detector::ConstructSDandField() { 
  G4ThreeVector fieldValue;
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);
  
  G4AutoDelete::Register(fMagFieldMessenger);
}
