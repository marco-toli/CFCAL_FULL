#include "EventAction.hh"



EventAction::EventAction(const DetectorConstruction* detectorConstruction):
  fDetectorConstruction(detectorConstruction)
{}



EventAction::~EventAction()
{}



void EventAction::BeginOfEventAction(const G4Event* evt)
{
  G4int evtNb = evt->GetEventID();
  if( evtNb%1 == 0 ) 
  {
    G4cout << "---> Begin of Event: " << evtNb << G4endl;
  }
  
  
  int nFib_x = fDetectorConstruction->GetNModules_x() * fDetectorConstruction->GetNFibers_x() * 2;
  int nFib_y = fDetectorConstruction->GetNModules_y() * fDetectorConstruction->GetNFibers_y() * 2;
  int nFib_z = fDetectorConstruction->GetNLayers_z() * 2;
  int nFib_xz = nFib_x * nFib_z / 2;
  int nFib_yz = nFib_y * nFib_z / 2;
  CreateTree::Instance() -> Clear(nFib_x,nFib_y,nFib_z,nFib_xz,nFib_yz);
  
  
  // --------------------- STORE INFO FOR X_0 / R_M ----------------------------- //
  int Radial_nSteps       = 5000;
  int Longitudinal_nSteps = 1000;
  CreateTree::Instance() -> Radial_stepLength       = 1000. / Radial_nSteps;       // in mm
  CreateTree::Instance() -> Longitudinal_stepLength = 5000. / Longitudinal_nSteps; // in mm
  
  // --------------------- STORE PRIMARY VERTEX ----------------------------- //
  
  if(CreateTree::Instance() -> Init_data())
  {
    G4double InitEnergy   = evt -> GetPrimaryVertex() -> GetPrimary() -> GetTotalEnergy();
    G4ThreeVector InitPos = evt -> GetPrimaryVertex() -> GetPosition();
    G4ThreeVector InitDir = evt -> GetPrimaryVertex() -> GetPrimary() -> GetMomentumDirection();
    
    CreateTree::Instance() -> InitialEnergy = InitEnergy/GeV;
    
    CreateTree::Instance() -> InitialPositionX = InitPos[0]/mm;
    CreateTree::Instance() -> InitialPositionY = InitPos[1]/mm;
    CreateTree::Instance() -> InitialPositionZ = InitPos[2]/mm;
    
    CreateTree::Instance() -> InitalMomentumDirectionX = InitDir[0];
    CreateTree::Instance() -> InitalMomentumDirectionY = InitDir[1];
    CreateTree::Instance() -> InitalMomentumDirectionZ = InitDir[2];
  }
    
  // -------------------- INSTANCE RUN/EVENT IN TREE ---------------------- //
  CreateTree::Instance() -> Event = evt -> GetEventID();
}

void EventAction::EndOfEventAction(const G4Event* evt)
{ 
  CreateTree::Instance()->Fill();
}

