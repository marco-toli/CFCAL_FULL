#include "SteppingAction.hh"

#include "G4SteppingManager.hh"
#include "G4SDManager.hh"
#include "G4EventManager.hh"
#include "EventAction.hh"
#include "G4ProcessManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4UnitsTable.hh"
#include "CreateTree.hh"
#include "MyMaterials.hh"

#include <iostream>
#include <fstream>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"



SteppingAction::SteppingAction()
{}



SteppingAction::~SteppingAction()
{}



void SteppingAction::UserSteppingAction(const G4Step * theStep)
{
  G4Track* theTrack = theStep->GetTrack();
  G4ParticleDefinition* particleType = theTrack->GetDefinition();
  
  
  G4StepPoint* thePrePoint  = theStep->GetPreStepPoint();
  G4StepPoint* thePostPoint = theStep->GetPostStepPoint();
  G4VPhysicalVolume* thePrePV  = thePrePoint ->GetPhysicalVolume();
  G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();
  G4String thePrePVName  = ""; if( thePrePV )  thePrePVName  = thePrePV  -> GetName();
  G4String thePostPVName = ""; if( thePostPV ) thePostPVName = thePostPV -> GetName();
  
  
  std::string iXLabel = "-1";
  std::string iYLabel = "-1";
  std::size_t pos;
  pos = thePrePVName.find("_x");    
  if( pos != std::string::npos ) iXLabel = thePrePVName.substr(pos+2,3);
  pos = thePrePVName.find("_y");    
  if( pos != std::string::npos ) iYLabel = thePrePVName.substr(pos+2,3);
  int iF_X = atoi(iXLabel.c_str());
  int iF_Y = atoi(iYLabel.c_str());
  
  
  
  
  // optical photon
  if( particleType == G4OpticalPhoton::OpticalPhotonDefinition() )
  { 
    G4String processName = theTrack->GetCreatorProcess()->GetProcessName();
    
    if( (CreateTree::Instance()->TIMING && processName != "Cerenkov") ||
        (!CreateTree::Instance()->TIMING) )
      theTrack->SetTrackStatus(fKillTrackAndSecondaries);
    
    
    
    //---------------------------------------------------------
    // storing time, energy and position of all optical photons
    
    G4int nStep = theTrack -> GetCurrentStepNumber();
    
    if( (theTrack->GetLogicalVolumeAtVertex()->GetName() == "Crystal_fiber_log") && (nStep == 1) )
    {
      G4String processName = theTrack->GetCreatorProcess()->GetProcessName();
      if     ( processName == "Cerenkov" )      CreateTree::Instance()->opPhoton_process.push_back( +1 );
      else if( processName == "Scintillation" ) CreateTree::Instance()->opPhoton_process.push_back( +2 );
      else                                      CreateTree::Instance()->opPhoton_process.push_back( -1 );
      
      CreateTree::Instance()->opPhoton_n++;
      CreateTree::Instance()->opPhoton_trackID.push_back( float(theTrack->GetTrackID()) );
      CreateTree::Instance()->opPhoton_parentTrackID.push_back( float(theTrack->GetParentID()) );
      CreateTree::Instance()->opPhoton_fiberIX.push_back( iF_X );
      CreateTree::Instance()->opPhoton_fiberIY.push_back( iF_Y );
      CreateTree::Instance()->opPhoton_energy.push_back( theTrack->GetTotalEnergy()/eV );
      CreateTree::Instance()->opPhoton_waveLength.push_back( MyMaterials::fromEvToNm(theTrack->GetTotalEnergy()/eV) );
      CreateTree::Instance()->opPhoton_time.push_back( thePrePoint->GetGlobalTime()/picosecond );
      CreateTree::Instance()->opPhoton_vertexX.push_back( thePrePoint->GetPosition().x()/cm );
      CreateTree::Instance()->opPhoton_vertexY.push_back( thePrePoint->GetPosition().y()/cm );
      CreateTree::Instance()->opPhoton_vertexZ.push_back( thePrePoint->GetPosition().z()/cm );
      CreateTree::Instance()->opPhoton_pX.push_back( theTrack->GetVertexMomentumDirection().x() );
      CreateTree::Instance()->opPhoton_pY.push_back( theTrack->GetVertexMomentumDirection().y() );
      CreateTree::Instance()->opPhoton_pZ.push_back( theTrack->GetVertexMomentumDirection().z() );
    }
    
    
    //----------------------------------------------
    // storing time, energy and position at detector
    
    G4bool thePrePVFound_front = false;
    pos = thePrePVName.find("Det_layer_front");
    if( pos != std::string::npos ) thePrePVFound_front = true;
    G4bool thePrePVFound_rear = false;
    pos = thePrePVName.find("Det_layer_rear");
    if( pos != std::string::npos ) thePrePVFound_rear = true;
    
    G4bool thePostPVFound_front = false;
    pos = thePostPVName.find("Det_front");
    if( pos != std::string::npos ) thePostPVFound_front = true;
    G4bool thePostPVFound_rear = false;
    pos = thePostPVName.find("Det_rear");
    if( pos != std::string::npos ) thePostPVFound_rear = true;
    
    if( (thePrePVFound_front == true && thePostPVFound_front == true) ||
        (thePrePVFound_rear  == true && thePostPVFound_rear  == true) )
    {
      if( theTrack->GetCreatorProcess() )
      {
        G4String processName = theTrack->GetCreatorProcess()->GetProcessName();
        if     ( processName == "Cerenkov" )      CreateTree::Instance()->opPhoton_process_det.push_back( +1 );
        else if( processName == "Scintillation" ) CreateTree::Instance()->opPhoton_process_det.push_back( +2 );
        else                                      CreateTree::Instance()->opPhoton_process_det.push_back( -1 );
      }
      
      CreateTree::Instance()->opPhoton_n_det++;
      CreateTree::Instance()->opPhoton_trackID_det.push_back( float(theTrack->GetTrackID()) );
      CreateTree::Instance()->opPhoton_parentTrackID_det.push_back( float(theTrack->GetParentID()) );
      CreateTree::Instance()->opPhoton_fiberIX_det.push_back( iF_X );
      CreateTree::Instance()->opPhoton_fiberIY_det.push_back( iF_Y );
      CreateTree::Instance()->opPhoton_energy_det.push_back( theTrack->GetTotalEnergy()/eV );
      CreateTree::Instance()->opPhoton_waveLength_det.push_back( MyMaterials::fromEvToNm(theTrack->GetTotalEnergy()/eV) );
      CreateTree::Instance()->opPhoton_time_det.push_back( theTrack->GetGlobalTime()/picosecond );
    }
  }
  
  
  // non optical photon
  else
  {
    G4TouchableHandle thePreTouch = thePrePoint -> GetTouchableHandle();
    G4VPhysicalVolume* thePreVolume = thePreTouch -> GetVolume();
    G4LogicalVolume* thePrePVLog = thePreVolume -> GetLogicalVolume();
    G4String thePrePVLogName = thePrePVLog -> GetName();
    
    
    Float_t energy = theStep->GetTotalEnergyDeposit()/GeV;
    Float_t nonion_energy = theStep->GetNonIonizingEnergyDeposit()/GeV;
    
    if (energy > 0)
    {	
      if( CreateTree::Instance()->Pos_fiber() )
      {
        G4ThreeVector pos = thePostPoint -> GetPosition();	
        CreateTree::Instance() -> depositionX.push_back(pos[0]);		
        CreateTree::Instance() -> depositionY.push_back(pos[1]);
        CreateTree::Instance() -> depositionZ.push_back(pos[2]);
        CreateTree::Instance() -> Energy_deposited.push_back(energy);
      }
      
      if( thePrePVName == "Box_abs_phys" || thePrePVLogName == "Crystal_fiber_log" )
        CreateTree::Instance()->Total_energy_absorber += energy;
      
      if( thePrePVLogName == "Crystal_fiber_log" )
      {
        CreateTree::Instance()->Total_energy_fibers += energy;
        CreateTree::Instance()->Total_nonion_energy_fibers += nonion_energy;
        
        if( CreateTree::Instance()->Energy_fiber() )
        {
          CreateTree::Instance()->Total_energy[iF_X][iF_Y] += energy;	
          CreateTree::Instance()->Total_nonion_energy[iF_X][iF_Y] += nonion_energy;	               
        }
      }
      
      CreateTree::Instance()->Total_energy_world += energy;       
    }  
  } // non optical photon
  
}
