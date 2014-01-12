//
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
//
// $Id: DetectorConstruction.hh,v 1.5 2006-06-29 17:53:55 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include "ConfigFile.hh"
#include "TString.h"

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  DetectorConstruction(const string& configFileName);
  ~DetectorConstruction();
  
  G4double GetHoleRadius()  const { return hole_radius; };
  G4double GetFiberRadius() const { return fiber_radius; };
  G4double GetFiberLength() const { return fiber_length; };
  
  G4double GetModule_x() const { return module_x; };
  G4double GetModule_y() const { return module_y; };
  G4double GetModule_z() const { return module_z; };
  
  G4double GetActiveArea_x() const { return activeArea_x; };
  G4double GetActiveArea_y() const { return activeArea_y; };
  
  G4int GetNModules_x() const { return nModules_x; };
  G4int GetNModules_y() const { return nModules_y; };
  
  G4int GetNFibers_x() const { return nFibers_x; };
  G4int GetNFibers_y() const { return nFibers_y; };
  G4int GetNLayers_z() const { return nLayers_z;  };
  
  G4double GetSpacing_x() const { return spacing_x; };
  G4double GetSpacing_y() const { return spacing_y; };
  G4double GetSpacing_z() const { return spacing_z; };
  
  G4double GetOffset_z()  const { return offset_z; };
  
  G4double GetPreshower_l()  const { return preshower_l; };
  
  G4double GetAttLambda(const G4int& it) const { return attLambdas[it]; };
  
public:
  G4VPhysicalVolume* Construct();
  
private:
  
  void readConfigFile(string configFileName);
  
  G4VPhysicalVolume* fAbsorberPV;   // the absorber physical volume
  
  G4VPhysicalVolume* fVHole1PV[500];   // the absorber holes
  G4VPhysicalVolume* fVHole2PV[500];   // the absorber holes
  G4VPhysicalVolume* fHHole1PV[500];   // the absorber holes
  G4VPhysicalVolume* fHHole2PV[500];   // the absorber holes
  
  G4VPhysicalVolume* fVFiber1PV[500];   // the absorber fibers
  G4VPhysicalVolume* fVFiber2PV[500];   // the absorber fibers
  G4VPhysicalVolume* fHFiber1PV[500];   // the absorber fibers
  G4VPhysicalVolume* fHFiber2PV[500];   // the absorber fibers
  
  G4VPhysicalVolume* fVTopGapLayer1PV[500];   // the absorber fibers
  G4VPhysicalVolume* fVTopGapLayer2PV[500];   // the absorber fibers
  G4VPhysicalVolume* fVBtmGapLayer1PV[500];   // the absorber fibers
  G4VPhysicalVolume* fVBtmGapLayer2PV[500];   // the absorber fibers
  G4VPhysicalVolume* fHTopGapLayer1PV[500];   // the absorber fibers
  G4VPhysicalVolume* fHTopGapLayer2PV[500];   // the absorber fibers
  G4VPhysicalVolume* fHBtmGapLayer1PV[500];   // the absorber fibers
  G4VPhysicalVolume* fHBtmGapLayer2PV[500];   // the absorber fibers
  
  G4VPhysicalVolume* fVTopGap1PV[500];   // the absorber fibers
  G4VPhysicalVolume* fVTopGap2PV[500];   // the absorber fibers
  G4VPhysicalVolume* fVBtmGap1PV[500];   // the absorber fibers
  G4VPhysicalVolume* fVBtmGap2PV[500];   // the absorber fibers
  G4VPhysicalVolume* fHTopGap1PV[500];   // the absorber fibers
  G4VPhysicalVolume* fHTopGap2PV[500];   // the absorber fibers
  G4VPhysicalVolume* fHBtmGap1PV[500];   // the absorber fibers
  G4VPhysicalVolume* fHBtmGap2PV[500];   // the absorber fibers
  
  G4VPhysicalVolume* fVTopWindow1PV[500];   // the absorber fibers
  G4VPhysicalVolume* fVTopWindow2PV[500];   // the absorber fibers
  G4VPhysicalVolume* fVBtmWindow1PV[500];   // the absorber fibers
  G4VPhysicalVolume* fVBtmWindow2PV[500];   // the absorber fibers
  G4VPhysicalVolume* fHTopWindow1PV[500];   // the absorber fibers
  G4VPhysicalVolume* fHTopWindow2PV[500];   // the absorber fibers
  G4VPhysicalVolume* fHBtmWindow1PV[500];   // the absorber fibers
  G4VPhysicalVolume* fHBtmWindow2PV[500];   // the absorber fibers
  
  G4VPhysicalVolume* fVTopDetLayer1PV[500];   // the absorber fibers
  G4VPhysicalVolume* fVTopDetLayer2PV[500];   // the absorber fibers
  G4VPhysicalVolume* fVBtmDetLayer1PV[500];   // the absorber fibers
  G4VPhysicalVolume* fVBtmDetLayer2PV[500];   // the absorber fibers
  G4VPhysicalVolume* fHTopDetLayer1PV[500];   // the absorber fibers
  G4VPhysicalVolume* fHTopDetLayer2PV[500];   // the absorber fibers
  G4VPhysicalVolume* fHBtmDetLayer1PV[500];   // the absorber fibers
  G4VPhysicalVolume* fHBtmDetLayer2PV[500];   // the absorber fibers
  
  G4VPhysicalVolume* fVTopDetector1PV[500];   // the absorber fibers
  G4VPhysicalVolume* fVTopDetector2PV[500];   // the absorber fibers
  G4VPhysicalVolume* fVBtmDetector1PV[500];   // the absorber fibers
  G4VPhysicalVolume* fVBtmDetector2PV[500];   // the absorber fibers
  G4VPhysicalVolume* fHTopDetector1PV[500];   // the absorber fibers
  G4VPhysicalVolume* fHTopDetector2PV[500];   // the absorber fibers
  G4VPhysicalVolume* fHBtmDetector1PV[500];   // the absorber fibers
  G4VPhysicalVolume* fHBtmDetector2PV[500];   // the absorber fibers
  
  G4VPhysicalVolume* fPreshowerPV;   // the preshower physical volume
  G4VPhysicalVolume* fPostshowerPV;   // the postshower physical volume
  G4VPhysicalVolume* fSideshowerPV;   // the postshower physical volume
  
  G4double expHall_x;
  G4double expHall_y;
  G4double expHall_z;
  
  G4double calor_x;
  G4double calor_y;
  G4double calor_z;
  
  G4int    crystal_material;
  G4int    crystal_lightyield;
  G4double crystal_risetime;
  G4double crystal_abslength;
  G4double crystal_induced_abslength;
  
  G4double fiber_length;
  G4double fiber_radius;
  G4double fiber_spacing_xy;
  G4double fiber_spacing_z;
  G4double fiber_offset_z;
  G4int fiber_x;
  G4int fiber_y;
  G4int nFibers_x;
  G4int nFibers_y;
  int nModules_x, nModules_y;
  int nLayers_z;
  G4double spacing_x, spacing_y, spacing_z;
  G4double offset_z;
  G4double hole_radius;
  
  G4double module_x;
  G4double module_y;
  G4double module_z;
  G4double activeArea_x;
  G4double activeArea_y;
  G4int abs_material;
  
  G4double win_r;
  G4double win_l;
  G4int    win_material;
  
  G4double gap_l;
  G4int    gap_material;
  
  G4double det_d;
  G4int    det_material;
  
  G4double preshower_l;
  G4int    preshower_material;
  G4double postshower_l;
  G4int    postshower_material;
  G4double sideshower_w;
  G4double sideshower_l;
  G4int    sideshower_material;
  
  G4double depth;
  
  G4double attLambdas[10];
  
  //Materials
  void initializeMaterials();
  G4Material* AbMaterial;
  G4Material* ScMaterial;
  G4Material* GaMaterial;
  G4Material* WiMaterial;
  G4Material* DeMaterial;
  G4Material* PreMaterial;
  G4Material* PostMaterial;
  G4Material* SideMaterial;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*DetectorConstruction_h*/
