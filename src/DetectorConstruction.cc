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
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction(const string& configFileName)
{
  readConfigFile(configFileName);
  expHall_x = expHall_y = expHall_z = 5*m;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  G4cout << ">>>>>> DetectorConstruction::Construct()::begin <<<<<<" << G4endl;
  
  
  initializeMaterials();
  
  
  //---------------------------------------
  //------------- Parameters --------------
  //---------------------------------------
  
  brass_hole_radius = 1.1*mm;
  fiber_radius = 1.*mm;
  fiber_lenght = absorber_z;
  
  
  
  //------------------------------------
  //------------- Volumes --------------
  //------------------------------------
  
  
  // The experimental Hall
  G4Box* expHall_box = new G4Box("World",expHall_x,expHall_y,expHall_z);
  G4LogicalVolume* expHall_log = new G4LogicalVolume(expHall_box,MyMaterials::Air(),"World",0,0,0);
  G4VPhysicalVolume* expHall_phys = new G4PVPlacement(0,G4ThreeVector(),expHall_log,"World",0,false,0);
  
  
  // assigning X,Y coordinates to each fiber
  G4double x [250][300];
  G4double y [250][300];
  
  G4cout << "NFIBERS_X: " << NFIBERS_X << "   NFIBERS_Y: " << NFIBERS_Y << std::endl;
  for (int iF_X = 0; iF_X < NFIBERS_X; iF_X++)
  {
    for(int iF_Y = 0; iF_Y < NFIBERS_Y; iF_Y++)
    {
      if( iF_Y%2 == 0 ) x[iF_X][iF_Y] = -(NFIBERS_X)/2*spacingX + iF_X*spacingX;
      else              x[iF_X][iF_Y] = -(NFIBERS_X)/2*spacingX + iF_X*spacingX + spacingX/2;
      
      y[iF_X][iF_Y] = (NFIBERS_Y)/2*spacingY - iF_Y*spacingY;
      G4cout << " x,y fiber[" << iF_X << "]["<<iF_Y<< "] = (" << x[iF_X][iF_Y] << "," << y[iF_X][iF_Y] << ")" << G4endl;
    }
  }
  
  
  // solids
  G4Box* Box_abs_solid = new G4Box("Box_abs_solid",0.5*absorber_x,0.5*absorber_y,0.7*absorber_z);
  G4Tubs* Brass_hole = new G4Tubs("Brass_hole",fiber_radius, brass_hole_radius,0.5*fiber_lenght,0.*deg,360.*deg);
  G4Tubs* Crystal_fiber = new G4Tubs("Crystal_fiber",0,fiber_radius,0.5*fiber_lenght,0.*deg,360.*deg);
  G4Tubs* Win_solid = new G4Tubs("Win_solid",0.0,win_r,0.5*win_l,0.*deg,360.*deg);
  G4Box* Det_layer_solid = new G4Box("Det_layer_solid",0.5*det_d,0.5*det_d,0.5*depth);
  G4Box* Det_solid = new G4Box("Det_solid",0.5*det_d,0.5*det_d,0.5*(det_d-depth));
  
  // logical
  G4LogicalVolume* Box_abs_log = new G4LogicalVolume(Box_abs_solid,AbMaterial, "Box_abs_log", 0,0,0);
  G4LogicalVolume* Brass_hole_log = new G4LogicalVolume(Brass_hole,MyMaterials::Air(),"Brass_hole_log",0,0,0);
  G4LogicalVolume* Crystal_fiber_log = new G4LogicalVolume(Crystal_fiber,ScMaterial,"Crystal_fiber_log",0,0,0);
  G4LogicalVolume* Win_log = new G4LogicalVolume(Win_solid,WiMaterial,"win_log",0,0,0);
  G4LogicalVolume* Det_layer_log = new G4LogicalVolume(Det_layer_solid,DeMaterial,"Det_layer_log",0,0,0);
  G4LogicalVolume* Det_log = new G4LogicalVolume(Det_solid,DeMaterial,"Det_log",0,0,0);
  
  // physical: placement 
  G4RotationMatrix *pRot = new G4RotationMatrix;
  pRot->rotateX(M_PI/2.*rad);
  
  G4VPhysicalVolume* Box_abs_phys = new G4PVPlacement(pRot, G4ThreeVector(0,0,0), Box_abs_log, "Box_abs_phys", expHall_log, false, 0);
  G4VPhysicalVolume* Brass_hole_phys[250][300];
  G4VPhysicalVolume* Crystal_phys[250][300];
  G4VPhysicalVolume* Win_front_phys[250][300];
  G4VPhysicalVolume* Win_rear_phys[250][300];
  G4VPhysicalVolume* Det_layer_front_phys[250][300];
  G4VPhysicalVolume* Det_layer_rear_phys[250][300];
  G4VPhysicalVolume* Det_front_phys[250][300];
  G4VPhysicalVolume* Det_rear_phys[250][300];
  
  char name[60];
  for (int iF_X = 0; iF_X < NFIBERS_X; iF_X++)
  {
    for (int iF_Y = 0; iF_Y < NFIBERS_Y; iF_Y++)
    {
      sprintf(name,"Hole_x%03d_y%03d",iF_X,iF_Y);
      Brass_hole_phys[iF_X][iF_Y] = new G4PVPlacement(0, G4ThreeVector(x[iF_X][iF_Y],y[iF_X][iF_Y],0), Brass_hole_log, name, Box_abs_log, false, 0);
      
      sprintf(name,"Fiber_x%03d_y%03d",iF_X,iF_Y);
      Crystal_phys[iF_X][iF_Y] = new G4PVPlacement(0, G4ThreeVector(x[iF_X][iF_Y],y[iF_X][iF_Y],0), Crystal_fiber_log, name, Box_abs_log, false, 0);     
      
      sprintf(name,"Win_front_x%03d_y%03d",iF_X,iF_Y);
      Win_front_phys[iF_X][iF_Y] = new G4PVPlacement(0, G4ThreeVector(x[iF_X][iF_Y],y[iF_X][iF_Y],0.5*fiber_lenght+det_distance+0.5*win_l), Win_log, name, Box_abs_log, false, 0);
      
      sprintf(name,"Win_rear_x%03d_y%03d",iF_X,iF_Y);
      Win_rear_phys[iF_X][iF_Y] = new G4PVPlacement(0, G4ThreeVector(x[iF_X][iF_Y],y[iF_X][iF_Y],-0.5*fiber_lenght-det_distance-0.5*win_l), Win_log, name, Box_abs_log, false, 0);
      
      sprintf(name,"Det_layer_front_x%03d_y%03d",iF_X,iF_Y);
      Det_layer_front_phys[iF_X][iF_Y] = new G4PVPlacement(0, G4ThreeVector(x[iF_X][iF_Y],y[iF_X][iF_Y],0.5*fiber_lenght+det_distance+win_l+0.5*depth), Det_layer_log, name, Box_abs_log, false, 0);
      
      sprintf(name,"Det_layer_rear_x%03d_y%03d",iF_X,iF_Y);
      Det_layer_rear_phys[iF_X][iF_Y] = new G4PVPlacement(0, G4ThreeVector(x[iF_X][iF_Y],y[iF_X][iF_Y],0.5*fiber_lenght+det_distance+win_l+0.5*depth), Det_layer_log, name, Box_abs_log, false, 0);
      
      sprintf(name,"Det_front_x%03d_y%03d",iF_X,iF_Y);
      Det_front_phys[iF_X][iF_Y] = new G4PVPlacement(0, G4ThreeVector(x[iF_X][iF_Y],y[iF_X][iF_Y],0.5*fiber_lenght+det_distance+win_l+depth+0.5*(det_d-depth)), Det_log, name, Box_abs_log, false, 0);
      
      sprintf(name,"Det_rear_x%03d_y%03d",iF_X,iF_Y);
      Det_rear_phys[iF_X][iF_Y] = new G4PVPlacement(0, G4ThreeVector(x[iF_X][iF_Y],y[iF_X][iF_Y],0.5*fiber_lenght+det_distance+win_l+depth+0.5*(det_d-depth)), Det_log, name, Box_abs_log, false, 0);
    }
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
  G4Colour  brown   (0.7, 0.4, 0.1) ;  // brass
  
  G4VisAttributes* VisAttWorld = new G4VisAttributes(white);
  VisAttWorld->SetVisibility(true);
  VisAttWorld->SetForceWireframe(true);
  expHall_log->SetVisAttributes(VisAttWorld);
  
  G4VisAttributes* VisAttAbs = new G4VisAttributes(brass);
  VisAttAbs->SetVisibility(true);
  VisAttAbs->SetForceWireframe(true);
  Box_abs_log->SetVisAttributes(VisAttAbs);
  
  G4VisAttributes* VisAttCrystal = new G4VisAttributes(green);
  VisAttCrystal->SetVisibility(true);
  VisAttCrystal->SetForceWireframe(false);
  Crystal_fiber_log->SetVisAttributes(VisAttCrystal);
  
  G4VisAttributes* VisAttWindow = new G4VisAttributes(blue);
  VisAttWindow->SetVisibility(true);
  VisAttWindow->SetForceWireframe(false);
  Win_log->SetVisAttributes(VisAttWindow);
  
  G4VisAttributes* VisAttDetector = new G4VisAttributes(gray);
  VisAttDetector->SetVisibility(true);
  VisAttDetector->SetForceWireframe(false);
  Det_log->SetVisAttributes(VisAttDetector);
  
  G4VisAttributes* VisAttHole = new G4VisAttributes(blue);
  VisAttHole->SetVisibility(false);
  Brass_hole_log->SetVisAttributes(VisAttHole);
  
  
  
  G4cout << ">>> DetectorConstruction::Construct()::endl <<< " << G4endl;
  return expHall_phys;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::readConfigFile(string configFileName)
{	
  ConfigFile config(configFileName);
  
  config.readInto(crystal_material,"crystal_material");
  config.readInto(crystal_risetime,"crystal_risetime");
  config.readInto(crystal_abslength,"crystal_abslength");
  config.readInto(crystal_lightyield,"crystal_lightyield");
  
  config.readInto(absorber_x,"absorber_x");
  config.readInto(absorber_y,"absorber_y");
  config.readInto(absorber_z,"absorber_z");
  config.readInto(spacingX,"spacingX");
  config.readInto(spacingY,"spacingY");
  config.readInto(NFIBERS_X,"NFIBERS_X");
  config.readInto(NFIBERS_Y,"NFIBERS_Y");
  config.readInto(abs_material,"abs_material");
  
  config.readInto(win_r,"win_r");
  config.readInto(win_l,"win_l");
  config.readInto(win_material,"win_material");
  
  config.readInto(det_d,"det_d");
  config.readInto(det_distance,"det_distance");
  config.readInto(det_material,"det_material");
  
  config.readInto(depth,"depth");
  
  // Crystal parameters
  /*
    G4double absorber_x = config.read<double>("absorber_x")*mm;
    G4cout << "Absorber x [mm]: " << absorber_x << G4endl;
    
    G4double absorber_y = config.read<double>("absorber_y")*mm;
    G4cout << "Absorber y [mm]: " << absorber_y << G4endl;
    
    G4double absorber_z = config.read<double>("absorber_z")*mm;
    G4cout << "Absorber z [mm]: " << absorber_z << G4endl;
    
    const int NFIBERS_X = config.read<int>("NFIBERS_X");
    G4cout << "NFIBERS_X: " << NFIBERS_X << G4endl;
    const int NFIBERS_Y = config.read<int>("NFIBERS_Y");
    G4cout << "NFIBERS_Y: " << NFIBERS_Y << G4endl;
    
    G4double spacingX = config.read<double>("spacingX")*mm;
    G4cout << "spacingX [mm]: " << spacingX << G4endl;
    G4double spacingY = config.read<double>("spacingY")*mm;
    G4cout << "spacingY [mm]: " << spacingY << G4endl;
  */
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::initializeMaterials()
{
  // define materials
  
  AbMaterial = NULL;
  if( abs_material == 1 ) AbMaterial = MyMaterials::Brass();
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
