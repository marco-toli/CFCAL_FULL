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
// $Id: DetectorConstruction.cc,v 1.18 2010-10-23 19:27:38 gum Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"

#include "G4Material.hh"
#include "MyMaterials.hh"
#include "G4Element.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction(const string& configFileName)
{
  readConfigFile(configFileName);
  
  
  
  //---------------------------------------
  //------------- Parameters --------------
  //---------------------------------------
  
  initializeMaterials();
  
  expHall_x = expHall_y = expHall_z = 10*m;
  
  hole_radius = fiber_radius + 0.1*mm;
  
  if( fiber_x )
  {
    module_y = fiber_length + 2.*(depth + gap_l + win_l + depth + det_d);
    activeArea_y = fiber_length;
  }
  else
  {
    module_y = (nFibers_y-0.5)*fiber_spacing_xy + 2.*hole_radius;
    activeArea_y = module_y;
    if( 0.5* det_d > hole_radius ) module_y += 2.*(0.5*det_d-hole_radius);
  }
  
  if( fiber_y )
  {
    module_x = fiber_length + 2.*(depth + gap_l + win_l + depth + det_d);
    activeArea_x = fiber_length;
  }
  else
  {
    module_x = (nFibers_x-0.5)*fiber_spacing_xy + 2.*hole_radius;
    activeArea_x = module_x;
    if( 0.5* det_d > hole_radius ) module_x += 2.*(0.5*det_d-hole_radius);
  }
  
  nFibers_x = int(activeArea_x / fiber_spacing_xy - 0.5) + 1;
  nFibers_y = int(activeArea_y / fiber_spacing_xy - 0.5) + 1;
  spacing_x = 2. * ( (activeArea_x-2.*hole_radius)/(2.*nFibers_x-1.) );
  spacing_y = 2. * ( (activeArea_y-2.*hole_radius)/(2.*nFibers_y-1.) );

  if( nLayers_z%2 ==1 ) nLayers_z += 1;
  spacing_z = fiber_spacing_z;
  module_z = (nLayers_z) * spacing_z;
  
  offset_z = fiber_offset_z;
  if( offset_z > +1.*(0.5*spacing_z-hole_radius) ) offset_z = +1. * (0.5*spacing_z-hole_radius);
  if( offset_z < -1.*(0.5*spacing_z-hole_radius) ) offset_z = -1. * (0.5*spacing_z-hole_radius);
  if( fiber_x && fiber_y && offset_z < hole_radius ) offset_z = hole_radius;
  
  
  calor_x = nModules_x * module_x;
  calor_y = nModules_y * module_y;
  calor_z = module_z;
  
  G4cout << "\n------------------------------------------------------------"
         << "\n---> The calorimeter is " << nLayers_z << " layers of: " << G4endl;
  G4cout << "------> absorber: " << spacing_z/mm << " mm of " << AbMaterial->GetName() << G4endl;
  if( fiber_x )
    G4cout << "------>   fibers: "   << nFibers_x << " vertical fibers (along the y axis) with a spacing of "<< spacing_x/mm << " mm" << G4endl;
  if( fiber_y )
    G4cout << "------>   fibers: "   << nFibers_y << " horizontal fibers (along the x axis) with a spacing of "<< spacing_y/mm << " mm" << G4endl;
  G4cout << "---> The total calorimeter dimensions are [" << calor_x << " x " << calor_y << " x " << calor_z << "] mm " << G4endl;
  G4cout << "---> The total active area dimensions are [" << activeArea_x << " x " << activeArea_y << " x " << calor_z << "] mm " << G4endl;
  G4cout << "------------------------------------------------------------\n" << G4endl;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  G4cout << ">>>>>> DetectorConstruction::Construct()::begin <<<<<<" << G4endl;
  
  
  //------------------------------------
  //------------- Volumes --------------
  //------------------------------------
  G4RotationMatrix* halfPiRotX = new G4RotationMatrix;
  halfPiRotX->rotateX(M_PI/2.*rad);  
  G4RotationMatrix* halfPiRotY = new G4RotationMatrix;
  halfPiRotY->rotateY(M_PI/2.*rad);
  G4RotationMatrix* halfPiRotZ = new G4RotationMatrix;
  halfPiRotZ->rotateZ(M_PI/2.*rad);
  
  
  // The experimental Hall
  G4VSolid* worldS = new G4Box("World",0.5*expHall_x,0.5*expHall_y,0.5*expHall_z);
  G4LogicalVolume* worldLV = new G4LogicalVolume(worldS,MyMaterials::Air(),"World",0,0,0);
  G4VPhysicalVolume* worldPV = new G4PVPlacement(0,G4ThreeVector(),worldLV,"World",0,false,0,true);
  
  
  // The calorimeter
  G4VSolid* calorS = new G4Box("Calorimeter",0.5*calor_x,0.5*calor_y,0.5*calor_z);
  G4LogicalVolume* calorLV = new G4LogicalVolume(calorS,MyMaterials::Air(),"Calorimeter");
  new G4PVPlacement(0,G4ThreeVector(),calorLV,"Calorimeter",worldLV,false,0,true);
  
  
  // Matrix
  G4VSolid* matrixS = new G4Box("Matrix",0.5*calor_x,0.5*module_y,0.5*module_z);
  G4LogicalVolume* matrixLV = new G4LogicalVolume(matrixS,MyMaterials::Air(),"Matrix");
  new G4PVReplica("Matrix",matrixLV,calorLV,kYAxis,nModules_y,module_y);
  
  
  // Module
  G4VSolid* moduleS = new G4Box("Module",0.5*module_x,0.5*module_y,0.5*module_z);
  G4LogicalVolume* moduleLV = new G4LogicalVolume(moduleS,MyMaterials::Air(),"Module");
  new G4PVReplica("Module",moduleLV,matrixLV,kXAxis,nModules_x,module_x);
  
  
  // A layer
  G4VSolid* layerS = new G4Box("Layer",0.5*module_x,0.5*module_y,spacing_z);
  G4LogicalVolume* layerLV = new G4LogicalVolume(layerS,MyMaterials::Air(),"Layer");
  new G4PVReplica("Layer",layerLV,moduleLV,kZAxis,int(nLayers_z/2),2.*spacing_z);
  
  
  // Absorber
  G4VSolid* absorberS = new G4Box("Absorber",0.5*activeArea_x,0.5*activeArea_y,spacing_z);
  G4LogicalVolume* absorberLV = new G4LogicalVolume(absorberS,AbMaterial,"Absorber");
  fAbsorberPV = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),absorberLV,"Absorber",layerLV,false,0,true);
  
  
  // Holes
  G4VSolid* holeS = new G4Tubs("Hole",fiber_radius,hole_radius,0.5*fiber_length,0.*deg,360.*deg);
  G4LogicalVolume* holeLV = new G4LogicalVolume(holeS,MyMaterials::Air(),"Hole");
  
  if( fiber_x )
  {
    for(int iX = 0; iX < nFibers_x; ++iX)
    {
      G4double x1 = -0.5*activeArea_x + hole_radius + iX*spacing_x;
      G4double x2 = -0.5*activeArea_x+ hole_radius + (iX+0.5)*spacing_x;
      G4double y1 = 0.;
      G4double y2 = 0.;
      G4double z1 = -0.5*spacing_z - offset_z;
      G4double z2 = +0.5*spacing_z - offset_z;
      
      fVHole1PV[iX] = new G4PVPlacement(halfPiRotX,G4ThreeVector(x1,y1,z1),holeLV,"vHole1",absorberLV,false,0,false);
      fVHole2PV[iX] = new G4PVPlacement(halfPiRotX,G4ThreeVector(x2,y2,z2),holeLV,"vHole2",absorberLV,false,0,false);
    }
  }
  if( fiber_y )
  {
    for(int iY = 0; iY < nFibers_y; ++iY)
    {
      G4double x1 = 0.;
      G4double x2 = 0.;
      G4double y1 = -0.5*activeArea_y + hole_radius + iY*spacing_y;
      G4double y2 = -0.5*activeArea_y + hole_radius + (iY+0.5)*spacing_y;
      G4double z1 = -0.5*spacing_z + offset_z;
      G4double z2 = +0.5*spacing_z + offset_z;
      
      fHHole1PV[iY] = new G4PVPlacement(halfPiRotY,G4ThreeVector(x1,y1,z1),holeLV,"hHole1",absorberLV,false,0,false);
      fHHole2PV[iY] = new G4PVPlacement(halfPiRotY,G4ThreeVector(x2,y2,z2),holeLV,"hHole2",absorberLV,false,0,false);    
    }
  }
  
  
  // Fibers
  G4VSolid* fiberS = new G4Tubs("Fiber",0.,fiber_radius,0.5*fiber_length,0.*deg,360.*deg);
  G4LogicalVolume* fiberLV = new G4LogicalVolume(fiberS,ScMaterial,"Fiber");
  
  if( fiber_x )
  {
    for(int iX = 0; iX < nFibers_x; ++iX)
    {
      G4double x1 = -0.5*activeArea_x + hole_radius + iX*spacing_x;
      G4double x2 = -0.5*activeArea_x + hole_radius + (iX+0.5)*spacing_x;
      G4double y1 = 0.;
      G4double y2 = 0.;
      G4double z1 = -0.5*spacing_z - offset_z;
      G4double z2 = +0.5*spacing_z - offset_z;
      
      fVFiber1PV[iX] = new G4PVPlacement(halfPiRotX,G4ThreeVector(x1,y1,z1),fiberLV,"vFiber1",absorberLV,false,0,false);
      fVFiber2PV[iX] = new G4PVPlacement(halfPiRotX,G4ThreeVector(x2,y2,z2),fiberLV,"vFiber2",absorberLV,false,0,false);
    }
  }
  if( fiber_y )
  {
    for(int iY = 0; iY < nFibers_y; ++iY)
    {
      G4double x1 = 0.;
      G4double x2 = 0.;
      G4double y1 = -0.5*activeArea_y + hole_radius + iY*spacing_y;
      G4double y2 = -0.5*activeArea_y + hole_radius + (iY+0.5)*spacing_y;
      G4double z1 = -0.5*spacing_z + offset_z;
      G4double z2 = +0.5*spacing_z + offset_z;
      
      fHFiber1PV[iY] = new G4PVPlacement(halfPiRotY,G4ThreeVector(x1,y1,z1),fiberLV,"hFiber1",absorberLV,false,0,false);
      fHFiber2PV[iY] = new G4PVPlacement(halfPiRotY,G4ThreeVector(x2,y2,z2),fiberLV,"hFiber2",absorberLV,false,0,false);
    }
  }
  
  
  // Gaps
  G4VSolid* gapLayerS = NULL;
  G4VSolid* gapS = NULL;
  G4LogicalVolume* gapLayerLV = NULL;
  G4LogicalVolume* gapLV = NULL;
  if( gap_l > 0 )
  {
    gapLayerS = new G4Tubs("GapLayer",0.,fiber_radius,0.5*depth,        0.*deg,360.*deg);
    gapS      = new G4Tubs("Gap",     0.,fiber_radius,0.5*(gap_l-depth),0.*deg,360.*deg);
    
    gapLayerLV = new G4LogicalVolume(gapLayerS,GaMaterial,"GapLayer");
    gapLV      = new G4LogicalVolume(gapS,     GaMaterial,"Gap");
    
    if( fiber_x )
    {
      for(int iX = 0; iX < nFibers_x; ++iX)
      {
        G4double x1 = -0.5*activeArea_x + hole_radius + iX*spacing_x;
        G4double x2 = -0.5*activeArea_x + hole_radius + (iX+0.5)*spacing_x;
        G4double y1 = 0.5*activeArea_y + 0.5*depth;
        G4double y2 = 0.5*activeArea_y + 0.5*depth;
        G4double z1 = -0.5*spacing_z - offset_z;
        G4double z2 = +0.5*spacing_z - offset_z;
        
        fVTopGapLayer1PV[iX] = new G4PVPlacement(halfPiRotX,G4ThreeVector(x1,y1,z1),gapLayerLV,"vTopGapLayer1",layerLV,false,0,false);
        fVTopGapLayer2PV[iX] = new G4PVPlacement(halfPiRotX,G4ThreeVector(x2,y2,z2),gapLayerLV,"vTopGapLayer2",layerLV,false,0,false);
        
        y1 *= -1.;
        y2 *= -1.;
        
        fVBtmGapLayer1PV[iX] = new G4PVPlacement(halfPiRotX,G4ThreeVector(x1,y1,z1),gapLayerLV,"vBtmGapLayer1",layerLV,false,0,false);
        fVBtmGapLayer2PV[iX] = new G4PVPlacement(halfPiRotX,G4ThreeVector(x2,y2,z2),gapLayerLV,"vBtmGapLayer2",layerLV,false,0,false);
        
        y1 = 0.5*fiber_length + depth + 0.5*(gap_l-depth);
        y2 = 0.5*fiber_length + depth + 0.5*(gap_l-depth);
        
        fVTopGap1PV[iX] = new G4PVPlacement(halfPiRotX,G4ThreeVector(x1,y1,z1),gapLV,"vTopGap1",layerLV,false,0,false);
        fVTopGap2PV[iX] = new G4PVPlacement(halfPiRotX,G4ThreeVector(x2,y2,z2),gapLV,"vTopGap2",layerLV,false,0,false);
        
        y1 *= -1.;
        y2 *= -1.;
        
        fVBtmGap1PV[iX] = new G4PVPlacement(halfPiRotX,G4ThreeVector(x1,y1,z1),gapLV,"vBtmGap1",layerLV,false,0,false);
        fVBtmGap2PV[iX] = new G4PVPlacement(halfPiRotX,G4ThreeVector(x2,y2,z2),gapLV,"vBtmGap2",layerLV,false,0,false);
      }
    }
    if( fiber_y )
    {
      for(int iY = 0; iY < nFibers_y; ++iY)
      {
        G4double x1 = 0.5*activeArea_x + 0.5*depth;
        G4double x2 = 0.5*activeArea_x + 0.5*depth;
        G4double y1 = -0.5*activeArea_y + hole_radius + iY*spacing_y;
        G4double y2 = -0.5*activeArea_y + hole_radius + (iY+0.5)*spacing_y;
        G4double z1 = -0.5*spacing_z + offset_z;
        G4double z2 = +0.5*spacing_z + offset_z;
        
        fHTopGapLayer1PV[iY] = new G4PVPlacement(halfPiRotY,G4ThreeVector(x1,y1,z1),gapLayerLV,"hTopGapLayer1",layerLV,false,0,false);
        fHTopGapLayer2PV[iY] = new G4PVPlacement(halfPiRotY,G4ThreeVector(x2,y2,z2),gapLayerLV,"hTopGapLayer2",layerLV,false,0,false);
        
        x1 *= -1.;
        x2 *= -1.;
        
        fHBtmGapLayer1PV[iY] = new G4PVPlacement(halfPiRotY,G4ThreeVector(x1,y1,z1),gapLayerLV,"hBtmGapLayer1",layerLV,false,0,false);
        fHBtmGapLayer2PV[iY] = new G4PVPlacement(halfPiRotY,G4ThreeVector(x2,y2,z2),gapLayerLV,"hBtmGapLayer2",layerLV,false,0,false);
        
        x1 = 0.5*fiber_length + depth + 0.5*(gap_l-depth);
        x2 = 0.5*fiber_length + depth + 0.5*(gap_l-depth);
        
        fHTopGap1PV[iY] = new G4PVPlacement(halfPiRotY,G4ThreeVector(x1,y1,z1),gapLV,"hTopGap1",layerLV,false,0,false);
        fHTopGap2PV[iY] = new G4PVPlacement(halfPiRotY,G4ThreeVector(x2,y2,z2),gapLV,"hTopGap2",layerLV,false,0,false);
        
        x1 *= -1.;
        x2 *= -1.;
        
        fHBtmGap1PV[iY] = new G4PVPlacement(halfPiRotY,G4ThreeVector(x1,y1,z1),gapLV,"hBtmGap1",layerLV,false,0,false);
        fHBtmGap2PV[iY] = new G4PVPlacement(halfPiRotY,G4ThreeVector(x2,y2,z2),gapLV,"hBtmGap2",layerLV,false,0,false);
      }
    }
  }
  
  
  // Windows
  G4VSolid* windowS = new G4Tubs("Window",0.0,win_r,0.5*win_l,0.*deg,360.*deg);
  G4LogicalVolume* windowLV = new G4LogicalVolume(windowS,WiMaterial,"Window");
  
  if( fiber_x )
  {
    for(int iX = 0; iX < nFibers_x; ++iX)
    {
      G4double x1 = -0.5*activeArea_x + hole_radius + iX*spacing_x;
      G4double x2 = -0.5*activeArea_x + hole_radius + (iX+0.5)*spacing_x;
      G4double y1 = 0.5*activeArea_y + gap_l + 0.5*win_l;
      G4double y2 = 0.5*activeArea_y + gap_l + 0.5*win_l;
      G4double z1 = -0.5*spacing_z - offset_z;
      G4double z2 = +0.5*spacing_z - offset_z;
      
      fVTopWindow1PV[iX] = new G4PVPlacement(halfPiRotX,G4ThreeVector(x1,y1,z1),windowLV,"vTopWindow1",layerLV,false,0,false);
      fVTopWindow2PV[iX] = new G4PVPlacement(halfPiRotX,G4ThreeVector(x2,y2,z2),windowLV,"vTopWindow2",layerLV,false,0,false);
      
      y1 *= -1.;
      y2 *= -1.;
      
      fVBtmWindow1PV[iX] = new G4PVPlacement(halfPiRotX,G4ThreeVector(x1,y1,z1),windowLV,"vBtmWindow1",layerLV,false,0,false);
      fVBtmWindow2PV[iX] = new G4PVPlacement(halfPiRotX,G4ThreeVector(x2,y2,z2),windowLV,"vBtmWindow2",layerLV,false,0,false);
    }  
  }
  if( fiber_y )
  {
    for(int iY = 0; iY < nFibers_y; ++iY)
    {
      G4double x1 = 0.5*activeArea_x + gap_l + 0.5*win_l;
      G4double x2 = 0.5*activeArea_x + gap_l + 0.5*win_l;
      G4double y1 = -0.5*activeArea_y + hole_radius + iY*spacing_y;
      G4double y2 = -0.5*activeArea_y + hole_radius + (iY+0.5)*spacing_y;
      G4double z1 = -0.5*spacing_z + offset_z;
      G4double z2 = +0.5*spacing_z + offset_z;
      
      fHTopWindow1PV[iY] = new G4PVPlacement(halfPiRotY,G4ThreeVector(x1,y1,z1),windowLV,"hTopWindow1",layerLV,false,0,false);
      fHTopWindow2PV[iY] = new G4PVPlacement(halfPiRotY,G4ThreeVector(x2,y2,z2),windowLV,"hTopWindow2",layerLV,false,0,false);
      
      x1 *= -1.;
      x2 *= -1.;
      
      fHBtmWindow1PV[iY] = new G4PVPlacement(halfPiRotY,G4ThreeVector(x1,y1,z1),windowLV,"hBtmWindow1",layerLV,false,0,false);
      fHBtmWindow2PV[iY] = new G4PVPlacement(halfPiRotY,G4ThreeVector(x2,y2,z2),windowLV,"hBtmWindow2",layerLV,false,0,false);
    }
  }
  
  
  // Detectors
  G4VSolid* detLayerS = new G4Box("DetLayer",0.5*det_d,0.5*depth,0.5*det_d);
  G4VSolid* detectorS = new G4Box("Detector",0.5*det_d,0.5*det_d,0.5*det_d);
  G4LogicalVolume* detectorLV = new G4LogicalVolume(detectorS,DeMaterial,"Detector");
  G4LogicalVolume* detLayerLV = new G4LogicalVolume(detLayerS,DeMaterial,"DetLayer");
  
  if( fiber_x )
  {
    for(int iX = 0; iX < nFibers_x; ++iX)
    {
      G4double x1 = -0.5*activeArea_x + hole_radius+iX*spacing_x;
      G4double x2 = -0.5*activeArea_x + hole_radius+(iX+0.5)*spacing_x;
      G4double y1 = 0.5*activeArea_y + gap_l + win_l + 0.5*depth;
      G4double y2 = 0.5*activeArea_y + gap_l + win_l + 0.5*depth;
      G4double z1 = -0.5*spacing_z -offset_z;
      G4double z2 = +0.5*spacing_z -offset_z;
    
      fVTopDetLayer1PV[iX] = new G4PVPlacement(0,G4ThreeVector(x1,y1,z1),detLayerLV,"vTopDetLayer1",layerLV,false,0,false);
      fVTopDetLayer2PV[iX] = new G4PVPlacement(0,G4ThreeVector(x2,y2,z2),detLayerLV,"vTopDetLayer2",layerLV,false,0,false);
      
      y1 *= -1.;
      y2 *= -1.;
      
      fVBtmDetLayer1PV[iX] = new G4PVPlacement(0,G4ThreeVector(x1,y1,z1),detLayerLV,"vBtmDetLayer1",layerLV,false,0,false);
      fVBtmDetLayer2PV[iX] = new G4PVPlacement(0,G4ThreeVector(x2,y2,z2),detLayerLV,"vBtmDetLayer2",layerLV,false,0,false);
      
      y1 = 0.5*activeArea_y + gap_l + win_l + depth + 0.5*det_d;
      y2 = 0.5*activeArea_y + gap_l + win_l + depth + 0.5*det_d;
      
      fVTopDetector1PV[iX] = new G4PVPlacement(0,G4ThreeVector(x1,y1,z1),detectorLV,"vTopDetector1",layerLV,false,0,false);
      fVTopDetector2PV[iX] = new G4PVPlacement(0,G4ThreeVector(x2,y2,z2),detectorLV,"vTopDetector2",layerLV,false,0,false);
      
      y1 *= -1.;
      y2 *= -1.;
      
      fVBtmDetector1PV[iX] = new G4PVPlacement(0,G4ThreeVector(x1,y1,z1),detectorLV,"vBtmDetector1",layerLV,false,0,false);
      fVBtmDetector2PV[iX] = new G4PVPlacement(0,G4ThreeVector(x2,y2,z2),detectorLV,"vBtmDetector2",layerLV,false,0,false);
    }
  }
  if( fiber_y )
  {    
    for(int iY = 0; iY < nFibers_y; ++iY)
    {
      G4double x1 = 0.5*activeArea_x + gap_l + win_l + 0.5*depth;
      G4double x2 = 0.5*activeArea_x + gap_l + win_l + 0.5*depth;
      G4double y1 = -0.5*activeArea_y + hole_radius+iY*spacing_y;
      G4double y2 = -0.5*activeArea_y + hole_radius+(iY+0.5)*spacing_y;
      G4double z1 = -0.5*spacing_z + offset_z;
      G4double z2 = +0.5*spacing_z + offset_z;
      
      fHTopDetLayer1PV[iY] = new G4PVPlacement(halfPiRotZ,G4ThreeVector(x1,y1,z1),detLayerLV,"hTopDetLayer1",layerLV,false,0,false);
      fHTopDetLayer2PV[iY] = new G4PVPlacement(halfPiRotZ,G4ThreeVector(x2,y2,z2),detLayerLV,"hTopDetLayer2",layerLV,false,0,false);
      
      x1 *= -1.;
      x2 *= -1.;
      
      fHBtmDetLayer1PV[iY] = new G4PVPlacement(halfPiRotZ,G4ThreeVector(x1,y1,z1),detLayerLV,"hBtmDetLayer1",layerLV,false,0,false);
      fHBtmDetLayer2PV[iY] = new G4PVPlacement(halfPiRotZ,G4ThreeVector(x2,y2,z2),detLayerLV,"hBtmDetLayer2",layerLV,false,0,false);
      
      x1 = 0.5*activeArea_x + gap_l + win_l + depth + 0.5*det_d;
      x2 = 0.5*activeArea_x + gap_l + win_l + depth + 0.5*det_d;
      
      fHTopDetector1PV[iY] = new G4PVPlacement(halfPiRotZ,G4ThreeVector(x1,y1,z1),detectorLV,"hTopDetector1",layerLV,false,0,false);
      fHTopDetector2PV[iY] = new G4PVPlacement(halfPiRotZ,G4ThreeVector(x2,y2,z2),detectorLV,"hTopDetector2",layerLV,false,0,false);
      
      x1 *= -1.;
      x2 *= -1.;
      
      fHBtmDetector1PV[iY] = new G4PVPlacement(halfPiRotZ,G4ThreeVector(x1,y1,z1),detectorLV,"hBtmDetector1",layerLV,false,0,false);
      fHBtmDetector2PV[iY] = new G4PVPlacement(halfPiRotZ,G4ThreeVector(x2,y2,z2),detectorLV,"hBtmDetector2",layerLV,false,0,false);
    }
  }
  
  
  // Preshower
  G4VSolid* preshowerS = NULL;
  G4LogicalVolume* preshowerLV = NULL;
  
  if( preshower_l > 0 )
  {
    G4double preshower_x = nModules_x * module_x;
    G4double preshower_y = nModules_y * module_y;
    
    preshowerS = new G4Box("Preshower",0.5*preshower_x,0.5*preshower_y,0.5*preshower_l);
    preshowerLV = new G4LogicalVolume(preshowerS,PreMaterial,"Preshower");
    fPreshowerPV = new G4PVPlacement(0,G4ThreeVector(0.,0.,-0.5*(module_z+preshower_l)),preshowerLV,"Preshower",worldLV,false,0,true);
  }
  
  
  // Postshower
  G4VSolid* postshowerS = NULL;
  G4LogicalVolume* postshowerLV = NULL;
  
  if( postshower_l > 0 )
  {
    G4double postshower_x = nModules_x * module_x;
    G4double postshower_y = nModules_y * module_y;
    
    postshowerS = new G4Box("Postshower",0.5*postshower_x,0.5*postshower_y,0.5*postshower_l);
    postshowerLV = new G4LogicalVolume(postshowerS,PostMaterial,"Postshower");
    fPostshowerPV = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.5*(postshower_l+calor_z)),postshowerLV,"Postshower",worldLV,false,0,true);
  }
  
  
  // Sideshower
  G4VSolid* sideshowerS = NULL;
  G4LogicalVolume* sideshowerLV = NULL;
  
  if( sideshower_w > 0 )
  {
    G4VSolid* dummy1 = new G4Box("dummy1",0.5*calor_x+sideshower_w,0.5*calor_y+sideshower_w,0.5*(calor_z+sideshower_l));
    G4VSolid* dummy2 = new G4Box("dummy2",0.5*calor_x,0.5*calor_y,module_z+sideshower_l);
    
    sideshowerS = new G4SubtractionSolid("Sideshower",dummy1,dummy2);
    sideshowerLV = new G4LogicalVolume(sideshowerS,SideMaterial,"Sideshower");
    fSideshowerPV = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.5*sideshower_l),sideshowerLV,"Sideshower",worldLV,false,0,true);
  }
  
  
  //-----------------------------------------------------
  //------------- attenuation lenghts -------------------
  //-----------------------------------------------------
  
  attLambdas[0] = 10.;
  attLambdas[1] = 20.;
  attLambdas[2] = 30.;
  attLambdas[3] = 50.;
  attLambdas[4] = 100.;
  attLambdas[5] = 200.;
  attLambdas[6] = 300.;
  attLambdas[7] = 500.;
  attLambdas[8] = 1000.;
  
  
  
  //-----------------------------------------------------
  //------------- Visualization attributes --------------
  //-----------------------------------------------------
  
  G4Colour  white   (1.0, 1.0, 1.0) ;  // white
  G4Colour  gray    (0.5, 0.5, 0.5) ;  // gray
  G4Colour  black   (0.0, 0.0, 0.0) ;  // black
  G4Colour  red     (1.0, 0.0, 0.0) ;  // red
  G4Colour  green   (0.0, 1.0, 0.0) ;  // green
  G4Colour  blue    (0.0, 0.0, 1.0) ;  // blue
  G4Colour  cyan    (0.0, 1.0, 1.0) ;  // cyan
  G4Colour  magenta (1.0, 0.0, 1.0) ;  // magenta 
  G4Colour  yellow  (1.0, 1.0, 0.0) ;  // yellow
  G4Colour  brass   (0.8, 0.6, 0.4) ;  // brass
  G4Colour  brown   (0.7, 0.4, 0.1) ;  // brown
  
  G4VisAttributes* VisAttWorld = new G4VisAttributes(white);
  VisAttWorld->SetVisibility(true);
  VisAttWorld->SetForceWireframe(true);
  worldLV->SetVisAttributes(VisAttWorld);
  
  G4VisAttributes* VisAttCalor = new G4VisAttributes(white);
  VisAttCalor->SetVisibility(true);
  VisAttCalor->SetForceWireframe(true);
  calorLV->SetVisAttributes(VisAttCalor);
  
  G4VisAttributes* VisAttMatrix = new G4VisAttributes(red);
  VisAttMatrix->SetVisibility(false);
  VisAttMatrix->SetForceWireframe(true);
  matrixLV->SetVisAttributes(VisAttMatrix);
  
  G4VisAttributes* VisAttModule = new G4VisAttributes(red);
  VisAttModule->SetVisibility(true);
  VisAttModule->SetForceWireframe(true);
  moduleLV->SetVisAttributes(VisAttModule);
  
  G4VisAttributes* VisAttLayer = new G4VisAttributes(red);
  VisAttLayer->SetVisibility(true);
  VisAttLayer->SetForceWireframe(true);
  layerLV->SetVisAttributes(VisAttLayer);
  
  G4VisAttributes* VisAttAbsorber = new G4VisAttributes(brass);
  VisAttAbsorber->SetVisibility(true);
  VisAttAbsorber->SetForceWireframe(true);
  absorberLV->SetVisAttributes(VisAttAbsorber);
  
  G4VisAttributes* VisAttHole = new G4VisAttributes(brass);
  VisAttHole->SetVisibility(true);
  VisAttHole->SetForceWireframe(false);
  holeLV->SetVisAttributes(VisAttHole);
  
  G4VisAttributes* VisAttFiber = new G4VisAttributes(green);
  VisAttFiber->SetVisibility(true);
  VisAttFiber->SetForceWireframe(false);
  fiberLV->SetVisAttributes(VisAttFiber);
  
  if( gap_l > 0 )
  {
    G4VisAttributes* VisAttGapLayer = new G4VisAttributes(red);
    VisAttGapLayer->SetVisibility(true);
    VisAttGapLayer->SetForceWireframe(false);
    gapLayerLV->SetVisAttributes(VisAttGapLayer);
    
    G4VisAttributes* VisAttGap = new G4VisAttributes(cyan);
    VisAttGap->SetVisibility(true);
    VisAttGap->SetForceWireframe(false);
    gapLV->SetVisAttributes(VisAttGap);
  }
  
  G4VisAttributes* VisAttWindow = new G4VisAttributes(blue);
  VisAttWindow->SetVisibility(true);
  VisAttWindow->SetForceWireframe(false);
  windowLV->SetVisAttributes(VisAttWindow);
  
  G4VisAttributes* VisAttDetLayer = new G4VisAttributes(red);
  VisAttDetLayer->SetVisibility(true);
  VisAttDetLayer->SetForceWireframe(false);
  detLayerLV->SetVisAttributes(VisAttDetLayer);
  
  G4VisAttributes* VisAttDetector = new G4VisAttributes(gray);
  VisAttDetector->SetVisibility(true);
  VisAttDetector->SetForceWireframe(false);
  detectorLV->SetVisAttributes(VisAttDetector);
  
  if( preshower_l > 0 )
  {
    G4VisAttributes* VisAttPreshower = new G4VisAttributes(gray);
    VisAttPreshower->SetVisibility(true);
    VisAttPreshower->SetForceWireframe(false);
    preshowerLV->SetVisAttributes(VisAttPreshower);
  }
  
  if( postshower_l > 0 )
  {
    G4VisAttributes* VisAttPostshower = new G4VisAttributes(gray);
    VisAttPostshower->SetVisibility(true);
    VisAttPostshower->SetForceWireframe(false);
    postshowerLV->SetVisAttributes(VisAttPostshower);
  }
  
  if( sideshower_w > 0 )
  {
    G4VisAttributes* VisAttSideshower = new G4VisAttributes(gray);
    VisAttSideshower->SetVisibility(true);
    VisAttSideshower->SetForceWireframe(true);
    sideshowerLV->SetVisAttributes(VisAttSideshower);
  }
  
  
  
  G4cout << ">>>>>> DetectorConstruction::Construct()::end <<< " << G4endl;
  return worldPV;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::readConfigFile(string configFileName)
{	
  ConfigFile config(configFileName);
  
  config.readInto(crystal_material,"crystal_material");
  config.readInto(crystal_risetime,"crystal_risetime");
  config.readInto(crystal_abslength,"crystal_abslength");
  config.readInto(crystal_lightyield,"crystal_lightyield");
  
  config.readInto(fiber_radius,"fiber_radius");
  config.readInto(fiber_length,"fiber_length");
  config.readInto(fiber_spacing_xy,"fiber_spacing_xy");
  config.readInto(fiber_spacing_z,"fiber_spacing_z");
  config.readInto(fiber_offset_z,"fiber_offset_z");
  config.readInto(fiber_x,"fiber_x");
  config.readInto(fiber_y,"fiber_y");
  config.readInto(nFibers_x,"nFibers_x");
  config.readInto(nFibers_y,"nFibers_y");
  config.readInto(nModules_x,"nModules_x");
  config.readInto(nModules_y,"nModules_y");
  config.readInto(nLayers_z,"nLayers_z");
  config.readInto(abs_material,"abs_material");
  
  config.readInto(gap_l,"gap_l");
  config.readInto(gap_material,"gap_material");
  
  config.readInto(win_r,"win_r");
  config.readInto(win_l,"win_l");
  config.readInto(win_material,"win_material");
  
  config.readInto(det_d,"det_d");
  config.readInto(det_material,"det_material");
  
  config.readInto(preshower_l,"preshower_l");
  config.readInto(preshower_material,"preshower_material");
  config.readInto(postshower_l,"postshower_l");
  config.readInto(postshower_material,"postshower_material");
  config.readInto(sideshower_w,"sideshower_w");
  config.readInto(sideshower_l,"sideshower_l");
  config.readInto(sideshower_material,"sideshower_material");
    
  config.readInto(depth,"depth");
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::initializeMaterials()
{
  // define materials
  
  AbMaterial = NULL;
  if     ( abs_material == 1 ) AbMaterial = MyMaterials::Brass();
  else if( abs_material == 2 ) AbMaterial = MyMaterials::Tungsten();
  else if( abs_material == 3 ) AbMaterial = MyMaterials::Air();
  else
  {
    G4cerr << "<DetectorConstructioninitializeMaterials>: Invalid absorber material specifier " << abs_material << G4endl;
    exit(-1);
  }
  G4cout << "Ab. material: "<< AbMaterial << G4endl;
  
  
  ScMaterial = NULL;
  if     ( crystal_material == 1 ) ScMaterial = MyMaterials::LSO();
  else if( crystal_material == 2 ) ScMaterial = MyMaterials::LYSO();
  else if( crystal_material == 3 ) ScMaterial = MyMaterials::LuAG_Ce();  
  else if( crystal_material == 4 ) ScMaterial = MyMaterials::LuAG_Pr();
  else if( crystal_material == 5 ) ScMaterial = MyMaterials::PWO();
  else if( crystal_material == 6 ) ScMaterial = MyMaterials::Air();
  else if( crystal_material == 7 ) ScMaterial = MyMaterials::Quartz();
  else if( crystal_material == 8 ) ScMaterial = MyMaterials::DSB_Ce();
  else if( crystal_material == 9 ) ScMaterial = MyMaterials::SiO2_Ce();
  else
  {
    G4cerr << "<DetectorConstructioninitializeMaterials>: Invalid crystal material specifier " << crystal_material << G4endl;
    exit(-1);
  }
  G4cout << "Sc. material: "<< ScMaterial << G4endl;
  
  
  GaMaterial = NULL;
  if     ( gap_material == 1 ) GaMaterial = MyMaterials::Air();
  else if( gap_material == 2 ) GaMaterial = MyMaterials::OpticalGrease();
  else
  {
    G4cerr << "<DetectorConstructioninitializeMaterials>: Invalid gap material specifier " << gap_material << G4endl;
    exit(-1);
  }
  G4cout << "Gap material: " << gap_material << G4endl;
  
  
  WiMaterial = NULL;
  if( win_material == 1 ) WiMaterial = MyMaterials::Quartz();
  else
  {
    G4cerr << "<DetectorConstructioninitializeMaterials>: Invalid window material specifier " << win_material << G4endl;
    exit(-1);
  }
  G4cout << "Window material: " << win_material << G4endl;
  
  
  DeMaterial = NULL;
  if( det_material == 1 ) DeMaterial = MyMaterials::Silicon();
  else
  {
    G4cerr << "<DetectorConstructioninitializeMaterials>: Invalid detector material specifier " << det_material << G4endl;
    exit(-1);
  }
  G4cout << "Detector material: " << det_material << G4endl;	
  
  
  PreMaterial = NULL;
  if     ( preshower_material == 1 ) PreMaterial = MyMaterials::Brass();
  else if( preshower_material == 2 ) PreMaterial = MyMaterials::Tungsten();
  else if( preshower_material == 3 ) PreMaterial = MyMaterials::Air();
  else
  {
    G4cerr << "<DetectorConstructioninitializeMaterials>: Invalid preshower material specifier " << preshower_material << G4endl;
    exit(-1);
  }
  G4cout << "Pre. material: "<< PreMaterial << G4endl;
  
  
  PostMaterial = NULL;
  if     ( postshower_material == 1 ) PostMaterial = MyMaterials::Brass();
  else if( postshower_material == 2 ) PostMaterial = MyMaterials::Tungsten();
  else if( postshower_material == 3 ) PostMaterial = MyMaterials::Air();
  else
  {
    G4cerr << "<DetectorConstructioninitializeMaterials>: Invalid postshower material specifier " << postshower_material << G4endl;
    exit(-1);
  }
  G4cout << "Post. material: "<< PostMaterial << G4endl;
  
  
  SideMaterial = NULL;
  if     ( sideshower_material == 1 ) SideMaterial = MyMaterials::Brass();
  else if( sideshower_material == 2 ) SideMaterial = MyMaterials::Tungsten();
  else if( sideshower_material == 3 ) SideMaterial = MyMaterials::Air();
  else
  {
    G4cerr << "<DetectorConstructioninitializeMaterials>: Invalid sideshower material specifier " << sideshower_material << G4endl;
    exit(-1);
  }
  G4cout << "Side. material: "<< SideMaterial << G4endl;
  
  
  
  // modify default properties of the scintillator
  if( crystal_lightyield >= 0 )
  {
    ScMaterial->GetMaterialPropertiesTable()->RemoveConstProperty("SCINTILLATIONYIELD");
    ScMaterial->GetMaterialPropertiesTable()->AddConstProperty("SCINTILLATIONYIELD",crystal_lightyield/MeV);  
  } 
  
  if( crystal_risetime >= 0 )
  {
    ScMaterial->GetMaterialPropertiesTable()->RemoveConstProperty("FASTSCINTILLATIONRISETIME");
    ScMaterial->GetMaterialPropertiesTable()->AddConstProperty("FASTSCINTILLATIONRISETIME",crystal_risetime/ns);  
  } 
  
  if( crystal_abslength >= 0 )
  {
    ScMaterial->GetMaterialPropertiesTable()->RemoveProperty("ABSLENGTH");

    const G4int nEntries_ABS = 2;
    G4double PhotonEnergy_ABS[nEntries_ABS] = { 1.*eV, 10.*eV };
    G4double Absorption[nEntries_ABS] = { crystal_abslength*mm, crystal_abslength*mm };

    ScMaterial->GetMaterialPropertiesTable()->AddProperty("ABSLENGTH",PhotonEnergy_ABS,Absorption,nEntries_ABS);
  }
  else 
  {
    for(unsigned int j = 0; j < ScMaterial->GetMaterialPropertiesTable()->GetProperty("ABSLENGTH")->GetVectorLength();++j)
    {
      ScMaterial->GetMaterialPropertiesTable()->GetProperty("ABSLENGTH")->Energy(j);
    }
  }
}
