// Martin Goettlich @ DESY
//
 
#include "EventAction.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "G4SDManager.hh"
#include "MyMaterials.hh"
#include "CreateTree.hh"
#include "PrimaryGeneratorAction.hh"

#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"


EventAction::EventAction()
{
}
 
EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event* evt)
{
  G4int evtNb = evt->GetEventID();
  if( evtNb%1 == 0 ) 
  {
    G4cout << "---> Begin of Event: " << evtNb << G4endl;
  }
  
  CreateTree::Instance() -> Clear();
  
  // --------------------- STORE PRIMARY VERTEX ----------------------------- //
  
  if(CreateTree::Instance() -> Init_data())
  {
    G4ThreeVector InitPos = evt -> GetPrimaryVertex() -> GetPosition();
    G4ThreeVector InitDir = evt -> GetPrimaryVertex() -> GetPrimary() -> GetMomentumDirection();

    CreateTree::Instance() -> InitialPositionX = InitPos[0];		
    CreateTree::Instance() -> InitialPositionY = InitPos[1];		
    CreateTree::Instance() -> InitialPositionZ = InitPos[2];
  	
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
  //if (CreateTree::Instance()-> Total_energy[0] > 8)
  //{
      
  //}
  
}

