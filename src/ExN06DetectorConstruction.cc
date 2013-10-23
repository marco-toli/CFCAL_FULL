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
// $Id: ExN06DetectorConstruction.cc,v 1.18 2010-10-23 19:27:38 gum Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "ExN06DetectorConstruction.hh"

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

ExN06DetectorConstruction::ExN06DetectorConstruction(string configFileName)
{

  readConfigFile( configFileName);
  expHall_x = expHall_y = expHall_z = 5*m;
 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN06DetectorConstruction::~ExN06DetectorConstruction()
{  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* ExN06DetectorConstruction::Construct()
{


//  readConfigFile("TB_8mm_long.cfg");
//  readConfigFile( configFileName);

//  absorber_x = 5.*cm;
//  absorber_y = 170.*cm;	//20 		// just front face, back face is 30 mm
//  absorber_z = 22.*cm;	//22
  
  fiber_radius = 1.*mm;
  brass_hole_radius = 1.1*mm;
  fiber_lenght = absorber_z;
  startAngle = 0.*deg;
  spanningAngle = 360.*deg;
  
//	------------- Materials -------------

  G4double a, z, density;
  G4int nelements;
  
  // Quartz
   G4Material* Quartz = MyMaterials::Quartz();

// Air
// 
  G4Element* N = new G4Element("Nitrogen", "N", z=7 , a=14.01*g/mole);
  G4Element* O = new G4Element("Oxygen"  , "O", z=8 , a=16.00*g/mole);

  G4Material* Air = new G4Material("Air", density=1.29*mg/cm3, nelements=2);
  Air->AddElement(N, 70.*perCent);
  Air->AddElement(O, 30.*perCent);
  
// Brass
//
  G4Element* Cu = new G4Element("Copper",    "Cu", z=29., a=63.546*g/mole);
  G4Element* Zn = new G4Element("Zinc",    "Zn", z=30., a=65.409*g/mole);
  
  G4Material* Brass = new G4Material("Brass", density=8.73*g/cm3,2);
  Brass->AddElement(Cu,0.75);
  Brass->AddElement(Zn,0.25);
  
//LuAG:Ce  
//
  G4Element* Lu = new G4Element("Lutetium",  "Lu",  z=71, a=174.97*g/mole);
  G4Element* Al = new G4Element("Aluminum",  "Al",  z=27, a=28.09*g/mole);

  G4Material* LuAG = new G4Material("LuAG", density=6.7*g/cm3,3);
  LuAG->AddElement(Lu,3);
  LuAG->AddElement(Al,12);
  LuAG->AddElement(O,5);
  
  //  2 bands at 290nm (4.28eV) and 350nm (3.54eV) about 50% of the light in each.
  const G4int NUMENTRIES_1 = 8;
  G4double FAST_Energy[NUMENTRIES_1]    = {1.0 * eV, 3.4 * eV, 3.5 * eV, 3.60 * eV, 3.90 * eV, 3.91 * eV, 4.07 * eV, 4.08 * eV};
  G4double FAST_COMPONENT[NUMENTRIES_1] = {0.00  , 0.00,  1.00,  0.0,    0.0,    1.0,    0.0,    0.0    };

  const G4int NUMENTRIES_2 = 2;
  G4double RIND_Energy[NUMENTRIES_2]    = { 2. * eV, 6. * eV };
  G4double RIND_INDEX[NUMENTRIES_2]     = { 1.82, 1.82 };
  G4double ABS_Energy[NUMENTRIES_2]     = {  2. * eV, 6. * eV };
  G4double ABS_LENGTH[NUMENTRIES_2]     = { 138.*mm, 138.*mm};
  
  G4MaterialPropertiesTable *mt = new G4MaterialPropertiesTable();
  mt->AddProperty ("RINDEX",        RIND_Energy, RIND_INDEX,     NUMENTRIES_2);
  mt->AddProperty ("ABSLENGTH",     ABS_Energy,  ABS_LENGTH,     NUMENTRIES_2);
  mt->AddConstProperty ("FASTSCINTILLATIONRISETIME", 0.5 * ns);
  mt->AddConstProperty ("FASTTIMECONSTANT", 20.*ns);
  mt->AddConstProperty ("SCINTILLATIONYIELD", 0 / MeV);
  
  LuAG->SetMaterialPropertiesTable (mt);

//
//	------------- Volumes --------------
//
  
// The experimental Hall

  G4Box* expHall_box = new G4Box("World",expHall_x,expHall_y,expHall_z);
  G4LogicalVolume* expHall_log = new G4LogicalVolume(expHall_box,Air,"World",0,0,0);
  G4VPhysicalVolume* expHall_phys = new G4PVPlacement(0,G4ThreeVector(),expHall_log,"World",0,false,0);

// The absorber
 
 // defining fibers position
  //const int NFIBERS_X = 10;
//  const int NFIBERS_Y = 200;
  
  G4double x[250][300];
  G4double y[250][300];
  
//  cout << " fibers x = " << NFIBERS_X << endl;
//  cout << " fibers y = " << NFIBERS_Y << endl;
  
 
//  G4double spacing = 4*mm;
 // G4double spacingY = 8*mm;
  
//  int iY = 0, iX = 0;
  
    ///assigning X coordinate to each fiber
      for (int iF_X = 0; iF_X < NFIBERS_X; iF_X++) {

      	for(int iF_Y = 0; iF_Y < NFIBERS_Y; iF_Y++) {
      	
//             standard array
			   if (iF_Y%2 == 0) x[iF_X][iF_Y] = -(NFIBERS_X)/2*spacingX + iF_X*spacingX;             
               else x[iF_X][iF_Y] = -(NFIBERS_X)/2*spacingX + spacingX/2 + iF_X*spacingX;           
     
               y[iF_X][iF_Y] = (NFIBERS_Y)/2*spacingY - iF_Y*spacingY;
			   cout << " x,y fiber[" << iF_X << "]["<<iF_Y<< "] = (" << x[iF_X][iF_Y] << "," << y[iF_X][iF_Y] << ")" << endl;
         }
      }
    
    
        
  // solids
  G4Box* Box_abs_solid = new G4Box("Box_abs_solid",0.5*absorber_x,0.5*absorber_y,0.5*absorber_z);
  G4Tubs* Brass_hole = new G4Tubs("Brass_hole",fiber_radius, brass_hole_radius,0.5*fiber_lenght,startAngle,spanningAngle);
  G4Tubs* Crystal_fiber = new G4Tubs("Crystal_fiber",0,fiber_radius,0.5*fiber_lenght,startAngle,spanningAngle);
  
  // logical
  G4LogicalVolume* Box_abs_log = new G4LogicalVolume(Box_abs_solid,Brass, "Box_abs_log", 0,0,0);
  G4LogicalVolume* Brass_hole_log = new G4LogicalVolume(Brass_hole,Air,"Brass_hole_log",0,0,0);
  G4LogicalVolume* Crystal_fiber_log = new G4LogicalVolume(Crystal_fiber,LuAG,"Crystal_fiber_log",0,0,0);
  
  // physical: placement 
  G4RotationMatrix *pRot = new G4RotationMatrix;
  pRot->rotateX(M_PI/2.*rad);
  
  
  G4VPhysicalVolume* Box_abs_phys = new G4PVPlacement(pRot, G4ThreeVector(0,0,0), Box_abs_log, "Box_abs_phys", expHall_log, false, 0);
  G4VPhysicalVolume* Brass_hole_phys[250][300];
  G4VPhysicalVolume* Crystal_phys[250][300];
  char name[60];

  
  for (int iF_X = 0; iF_X < NFIBERS_X; iF_X++) {
    for (int iF_Y = 0; iF_Y < NFIBERS_Y; iF_Y++) {

	    sprintf (name, "Hole_x%d_y%d" , iF_X, iF_Y);
	    Brass_hole_phys[iF_X][iF_Y] = new G4PVPlacement(0, G4ThreeVector(x[iF_X][iF_Y],y[iF_X][iF_Y],0), Brass_hole_log, name, Box_abs_log, false, 0);
    
    	sprintf (name, "Fiber_x%d_y%d" , iF_X, iF_Y);
	    Crystal_phys[iF_X][iF_Y] = new G4PVPlacement(0, G4ThreeVector(x[iF_X][iF_Y],y[iF_X][iF_Y],0), Crystal_fiber_log, name, Box_abs_log, false, 0);     

    }
  }
    
  
  // 
  //******************************* VISUALIZATION ATTRIBUTES ******************************* //
  //  
    
  //G4Colour  white   ()              ;  // white
  G4Colour  white   (1.0, 1.0, 1.0) ;  // white
  G4Colour  gray    (0.5, 0.5, 0.5) ;  // gray
  G4Colour  black   (0.0, 0.0, 0.0) ;  // black
  G4Colour  red     (1.0, 0.0, 0.0) ;  // red
  G4Colour  green   (0.0, 1.0, 0.0) ;  // green
  G4Colour  blue    (0.0, 0.0, 1.0) ;  // blue
  G4Colour  cyan    (0.0, 1.0, 1.0) ;  // cyan
  G4Colour  magenta (1.0, 0.0, 1.0) ;  // magenta 
  G4Colour  yellow  (1.0, 1.0, 0.0) ;  // yellow
    
  G4VisAttributes* VisAttCrystal = new G4VisAttributes(red);
  VisAttCrystal->SetVisibility(true);
  //VisAttCrystal->SetColour(green);
  Crystal_fiber_log->SetVisAttributes(VisAttCrystal);
	
  G4VisAttributes* VisAttHole = new G4VisAttributes(blue);
  VisAttHole->SetVisibility(true);
  //VisAttCrystal->SetColour(green);
  Brass_hole_log->SetVisAttributes(VisAttHole);

  return expHall_phys;
}


void ExN06DetectorConstruction::readConfigFile(string configFileName)
{	

	ConfigFile config(configFileName);
	
	config.readInto(absorber_x,"absorber_x");
	config.readInto(absorber_y,"absorber_y");
	config.readInto(absorber_z,"absorber_z");

	config.readInto(spacingX,"spacingX");
	config.readInto(spacingY,"spacingY");
	
	config.readInto(NFIBERS_X,"NFIBERS_X");
	config.readInto(NFIBERS_Y,"NFIBERS_Y");
	
	
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
