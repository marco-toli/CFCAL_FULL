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
  
  if( nLayers_z%2 ==1 ) nLayers_z += 1;
  
  
  //---------------------------------------
  //------------- Parameters --------------
  //---------------------------------------
  
  initializeMaterials();
  
  expHall_x = expHall_y = expHall_z = 10*m;
  
  hole_radius = fiber_radius + 0.1*mm;
  
  module_x = fiber_length + 2.*(det_distance + win_l + depth + det_d);
  module_y = fiber_length + 2.*(det_distance + win_l + depth + det_d);
  module_z = (nLayers_z) * spacing_z;
  
  nFibers_xy = int(fiber_length / spacing_xy);
  spacing_xy = 2. * ((fiber_length-2.*fiber_radius) / (2.*nFibers_xy-1.) );
  
  G4cout << "\n------------------------------------------------------------"
         << "\n---> The calorimeter is " << nLayers_z << " layers of: " << G4endl;
  G4cout << "absorber: " << spacing_z/mm << " mm of " << AbMaterial->GetName() << G4endl;
  G4cout << "fibers: "   << nFibers_xy << " fibers along the x/y axis with a spacing of "<< spacing_xy/mm << " mm" << G4endl;
  G4cout << "\n------------------------------------------------------------\n" << G4endl;
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
  G4VSolid* calorS = new G4Box("Calorimeter",0.5*module_x*nModules_x,0.5*module_y*nModules_y,0.5*module_z);
  G4LogicalVolume* calorLV = new G4LogicalVolume(calorS,MyMaterials::Air(),"Calorimeter");
  new G4PVPlacement(0,G4ThreeVector(),calorLV,"Calorimeter",worldLV,false,0,true);
  
  
  // Matrix
  G4VSolid* matrixS = new G4Box("Matrix",0.5*module_x*nModules_x,0.5*module_y,0.5*module_z);
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
  G4VSolid* absorberS = new G4Box("Absorber",0.5*fiber_length,0.5*fiber_length,spacing_z);
  G4LogicalVolume* absorberLV = new G4LogicalVolume(absorberS,AbMaterial,"Absorber");
  fAbsorberPV = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),absorberLV,"Absorber",layerLV,false,0,true);
  
  
  // Holes
  G4VSolid* holeS = new G4Tubs("Hole",fiber_radius,hole_radius,0.5*fiber_length,0.*deg,360.*deg);
  G4LogicalVolume* holeLV = new G4LogicalVolume(holeS,MyMaterials::Air(),"Hole");
  
  for(int iX = 0; iX < nFibers_xy; ++iX)
  {
    G4double x1 = -0.5*fiber_length + hole_radius + iX*spacing_xy;
    G4double x2 = -0.5*fiber_length + hole_radius + (iX+0.5)*spacing_xy;
    G4double y1 = 0.;
    G4double y2 = 0.;
    G4double z1 = -0.5*spacing_z;
    G4double z2 = +0.5*spacing_z;
    
    fVHole1PV[iX] = new G4PVPlacement(halfPiRotX,G4ThreeVector(x1,y1,z1),holeLV,"vHole1",absorberLV,false,0,false);
    fVHole2PV[iX] = new G4PVPlacement(halfPiRotX,G4ThreeVector(x2,y2,z2),holeLV,"vHole2",absorberLV,false,0,false);
    
    x1 = 0.;
    x2 = 0.;
    y1 = -0.5*fiber_length + hole_radius + iX*spacing_xy;
    y2 = -0.5*fiber_length + hole_radius + (iX+0.5)*spacing_xy;
    z1 = -0.5*spacing_z + spacing_xy;
    z2 = +0.5*spacing_z + spacing_xy;
    
    fHHole1PV[iX] = new G4PVPlacement(halfPiRotY,G4ThreeVector(x1,y1,z1),holeLV,"hHole1",absorberLV,false,0,false);
    fHHole2PV[iX] = new G4PVPlacement(halfPiRotY,G4ThreeVector(x2,y2,z2),holeLV,"hHole2",absorberLV,false,0,false);    
  }
  
  
  // Fibers
  G4VSolid* fiberS = new G4Tubs("Fiber",0.,fiber_radius,0.5*fiber_length,0.*deg,360.*deg);
  G4LogicalVolume* fiberLV = new G4LogicalVolume(fiberS,ScMaterial,"Fiber");
  
  for(int iX = 0; iX < nFibers_xy; ++iX)
  {
    G4double x1 = -0.5*fiber_length + hole_radius + iX*spacing_xy;
    G4double x2 = -0.5*fiber_length + hole_radius + (iX+0.5)*spacing_xy;
    G4double y1 = 0.;
    G4double y2 = 0.;
    G4double z1 = -0.5*spacing_z;
    G4double z2 = +0.5*spacing_z;
    
    fVFiber1PV[iX] = new G4PVPlacement(halfPiRotX,G4ThreeVector(x1,y1,z1),fiberLV,"VFiber1",absorberLV,false,0,false);
    fVFiber2PV[iX] = new G4PVPlacement(halfPiRotX,G4ThreeVector(x2,y2,z2),fiberLV,"VFiber2",absorberLV,false,0,false);
    
    x1 = 0.;
    x2 = 0.;
    y1 = -0.5*fiber_length + hole_radius + iX*spacing_xy;
    y2 = -0.5*fiber_length + hole_radius + (iX+0.5)*spacing_xy;
    z1 = -0.5*spacing_z + spacing_xy;
    z2 = +0.5*spacing_z + spacing_xy;
    
    fHFiber1PV[iX] = new G4PVPlacement(halfPiRotY,G4ThreeVector(x1,y1,z1),fiberLV,"HFiber1",absorberLV,false,0,false);
    fHFiber1PV[iX] = new G4PVPlacement(halfPiRotY,G4ThreeVector(x2,y2,z2),fiberLV,"HFiber2",absorberLV,false,0,false);
  }
  
  
  // Windows
  G4VSolid* windowS = new G4Tubs("Window",0.0,win_r,0.5*win_l,0.*deg,360.*deg);
  G4LogicalVolume* windowLV = new G4LogicalVolume(windowS,WiMaterial,"Window");
  
  for(int iX = 0; iX < nFibers_xy; ++iX)
  {
    G4double x1 = -0.5*fiber_length + hole_radius + iX*spacing_xy;
    G4double x2 = -0.5*fiber_length + hole_radius + (iX+0.5)*spacing_xy;
    G4double y1 = 0.5*fiber_length + det_distance + 0.5*win_l;
    G4double y2 = 0.5*fiber_length + det_distance + 0.5*win_l;
    G4double z1 = -0.5*spacing_z;
    G4double z2 = +0.5*spacing_z;
    
    fVTopWindow1PV[iX] = new G4PVPlacement(halfPiRotX,G4ThreeVector(x1,y1,z1),windowLV,"vWindow1",absorberLV,false,0,false);
    fVTopWindow2PV[iX] = new G4PVPlacement(halfPiRotX,G4ThreeVector(x2,y2,z2),windowLV,"vWindow2",absorberLV,false,0,false);
    
    y1 *= -1.;
    y2 *= -1.;
    
    fVBtmWindow1PV[iX] = new G4PVPlacement(halfPiRotX,G4ThreeVector(x1,y1,z1),windowLV,"vWindow1",layerLV,false,0,false);
    fVBtmWindow2PV[iX] = new G4PVPlacement(halfPiRotX,G4ThreeVector(x2,y2,z2),windowLV,"vWindow2",layerLV,false,0,false);
    
    
    x1 = 0.5*fiber_length + det_distance + 0.5*win_l;
    x2 = 0.5*fiber_length + det_distance + 0.5*win_l;
    y1 = -0.5*fiber_length + hole_radius + iX*spacing_xy;
    y2 = -0.5*fiber_length + hole_radius + (iX+0.5)*spacing_xy;
    z1 = -0.5*spacing_z + spacing_xy;
    z2 = +0.5*spacing_z + spacing_xy;
    
    fHTopWindow1PV[iX] = new G4PVPlacement(halfPiRotY,G4ThreeVector(x1,y1,z1),windowLV,"hWindow1",absorberLV,false,0,false);
    fHTopWindow2PV[iX] = new G4PVPlacement(halfPiRotY,G4ThreeVector(x2,y2,z2),windowLV,"hWindow2",absorberLV,false,0,false);
    
    x1 *= -1.;
    x2 *= -1.;
    
    fHBtmWindow1PV[iX] = new G4PVPlacement(halfPiRotY,G4ThreeVector(x1,y1,z1),windowLV,"hWindow1",layerLV,false,0,false);
    fHBtmWindow2PV[iX] = new G4PVPlacement(halfPiRotY,G4ThreeVector(x2,y2,z2),windowLV,"hWindow2",layerLV,false,0,false);
  }
  
  
  // Detectors
  G4VSolid* detLayerS = new G4Box("DetLayer",0.5*det_d,0.5*depth,0.5*det_d);
  G4VSolid* detectorS = new G4Box("Detector",0.5*det_d,0.5*det_d,0.5*det_d);
  G4LogicalVolume* detectorLV = new G4LogicalVolume(detectorS,DeMaterial,"Detector");
  G4LogicalVolume* detLayerLV = new G4LogicalVolume(detLayerS,DeMaterial,"DetLayer");
  
  for(int iX = 0; iX < nFibers_xy; ++iX)
  {
    G4double x1 = -0.5*fiber_length + hole_radius+iX*spacing_xy;
    G4double x2 = -0.5*fiber_length + hole_radius+(iX+0.5)*spacing_xy;
    G4double y1 = 0.5*fiber_length + det_distance + win_l + 0.5*depth;
    G4double y2 = 0.5*fiber_length + det_distance + win_l + 0.5*depth;
    G4double z1 = -0.5*spacing_z;
    G4double z2 = +0.5*spacing_z;
    
    fVTopDetLayer1PV[iX] = new G4PVPlacement(0,G4ThreeVector(x1,y1,z1),detLayerLV,"vTopDetLayer1",layerLV,false,0,false);
    fVTopDetLayer2PV[iX] = new G4PVPlacement(0,G4ThreeVector(x2,y2,z2),detLayerLV,"vTopDetLayer2",layerLV,false,0,false);
    
    y1 *= -1.;
    y2 *= -1.;
    
    fVBtmDetLayer1PV[iX] = new G4PVPlacement(0,G4ThreeVector(x1,y1,z1),detLayerLV,"vBtmDetLayer1",layerLV,false,0,false);
    fVBtmDetLayer2PV[iX] = new G4PVPlacement(0,G4ThreeVector(x2,y2,z2),detLayerLV,"vBtmDetLayer2",layerLV,false,0,false);
    
    
    y1 = 0.5*fiber_length + det_distance + win_l + depth + 0.5*det_d;
    y2 = 0.5*fiber_length + det_distance + win_l + depth + 0.5*det_d;
    
    fVTopDetector1PV[iX] = new G4PVPlacement(0,G4ThreeVector(x1,y1,z1),detectorLV,"vTopDetector1",layerLV,false,0,false);
    fVTopDetector2PV[iX] = new G4PVPlacement(0,G4ThreeVector(x2,y2,z2),detectorLV,"vTopDetector2",layerLV,false,0,false);
    
    y1 *= -1.;
    y2 *= -1.;
    
    fVBtmDetector1PV[iX] = new G4PVPlacement(0,G4ThreeVector(x1,y1,z1),detectorLV,"vBtmDetector1",layerLV,false,0,false);
    fVBtmDetector2PV[iX] = new G4PVPlacement(0,G4ThreeVector(x2,y2,z2),detectorLV,"vBtmDetector2",layerLV,false,0,false);
    
    
    
    x1 = 0.5*fiber_length + det_distance + win_l + 0.5*depth;
    x2 = 0.5*fiber_length + det_distance + win_l + 0.5*depth;
    y1 = -0.5*fiber_length + hole_radius+iX*spacing_xy;
    y2 = -0.5*fiber_length + hole_radius+(iX+0.5)*spacing_xy;
    z1 = -0.5*spacing_z + spacing_xy;
    z2 = +0.5*spacing_z + spacing_xy;
    
    fHTopDetLayer1PV[iX] = new G4PVPlacement(halfPiRotZ,G4ThreeVector(x1,y1,z1),detLayerLV,"hTopDetLayer1",layerLV,false,0,false);
    fHTopDetLayer2PV[iX] = new G4PVPlacement(halfPiRotZ,G4ThreeVector(x2,y2,z2),detLayerLV,"hTopDetLayer2",layerLV,false,0,false);
    
    x1 *= -1.;
    x2 *= -1.;
    
    fHBtmDetLayer1PV[iX] = new G4PVPlacement(halfPiRotZ,G4ThreeVector(x1,y1,z1),detLayerLV,"hBtmDetLayer1",layerLV,false,0,false);
    fHBtmDetLayer2PV[iX] = new G4PVPlacement(halfPiRotZ,G4ThreeVector(x2,y2,z2),detLayerLV,"hBtmDetLayer2",layerLV,false,0,false);
    
    
    x1 = 0.5*fiber_length + det_distance + win_l + depth + 0.5*det_d;
    x2 = 0.5*fiber_length + det_distance + win_l + depth + 0.5*det_d;
    
    fHTopDetector1PV[iX] = new G4PVPlacement(halfPiRotZ,G4ThreeVector(x1,y1,z1),detectorLV,"hTopDetector1",layerLV,false,0,false);
    fHTopDetector2PV[iX] = new G4PVPlacement(halfPiRotZ,G4ThreeVector(x2,y2,z2),detectorLV,"hTopDetector2",layerLV,false,0,false);
    
    x1 *= -1.;
    x2 *= -1.;
    
    fHBtmDetector1PV[iX] = new G4PVPlacement(halfPiRotZ,G4ThreeVector(x1,y1,z1),detectorLV,"hBtmDetector1",layerLV,false,0,false);
    fHBtmDetector2PV[iX] = new G4PVPlacement(halfPiRotZ,G4ThreeVector(x2,y2,z2),detectorLV,"hBtmDetector2",layerLV,false,0,false);
  }
  
  
  
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
  
  G4VisAttributes* VisAttHole = new G4VisAttributes(blue);
  VisAttHole->SetVisibility(false);
  VisAttHole->SetForceWireframe(false);
  holeLV->SetVisAttributes(VisAttHole);
  
  G4VisAttributes* VisAttFiber = new G4VisAttributes(green);
  VisAttFiber->SetVisibility(true);
  VisAttFiber->SetForceWireframe(false);
  fiberLV->SetVisAttributes(VisAttFiber);
  
  G4VisAttributes* VisAttDetLayer = new G4VisAttributes(red);
  VisAttDetLayer->SetVisibility(false);
  VisAttDetLayer->SetForceWireframe(false);
  detLayerLV->SetVisAttributes(VisAttDetLayer);
  
  G4VisAttributes* VisAttDetector = new G4VisAttributes(gray);
  VisAttDetector->SetVisibility(false);
  VisAttDetector->SetForceWireframe(false);
  detectorLV->SetVisAttributes(VisAttDetector);
  
  
  
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
  config.readInto(spacing_xy,"spacing_xy");
  config.readInto(spacing_z,"spacing_z");
  config.readInto(nModules_x,"nModules_x");
  config.readInto(nModules_y,"nModules_y");
  config.readInto(nLayers_z,"nLayers_z");
  config.readInto(abs_material,"abs_material");
  
  config.readInto(win_r,"win_r");
  config.readInto(win_l,"win_l");
  config.readInto(win_material,"win_material");
  
  config.readInto(det_d,"det_d");
  config.readInto(det_distance,"det_distance");
  config.readInto(det_material,"det_material");
  
  config.readInto(depth,"depth");
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::initializeMaterials()
{
  // define materials
  
  AbMaterial = NULL;
  if( abs_material == 1 ) AbMaterial = MyMaterials::Brass();
  else if( abs_material == 2 ) AbMaterial = MyMaterials::Tungsten();
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
    ScMaterial->GetMaterialPropertiesTable()->AddConstProperty("ABSLENGTH",crystal_abslength);  
  } 
  else 
  {
    for(unsigned int j = 0; j < ScMaterial->GetMaterialPropertiesTable()->GetProperty("ABSLENGTH")->GetVectorLength();++j)
    {
      ScMaterial->GetMaterialPropertiesTable()->GetProperty("ABSLENGTH")->Energy(j);
    }
  }
}
