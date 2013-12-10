#include "SteppingAction.hh"




SteppingAction::SteppingAction(const DetectorConstruction* detectorConstruction):
  fDetectorConstruction(detectorConstruction)
{}



SteppingAction::~SteppingAction()
{}



void SteppingAction::UserSteppingAction(const G4Step * theStep)
{
  G4StepPoint* thePrePoint  = theStep -> GetPreStepPoint();
  G4StepPoint* thePostPoint = theStep -> GetPostStepPoint();
  
  G4VPhysicalVolume* thePrePV  = thePrePoint  -> GetPhysicalVolume();
  G4VPhysicalVolume* thePostPV = thePostPoint -> GetPhysicalVolume();
  
  if( !thePostPV ) return; // OutOfWorld
  
  G4LogicalVolume* thePreLV  = thePrePV  -> GetLogicalVolume();
  G4LogicalVolume* thePostLV = thePostPV -> GetLogicalVolume();
  G4String thePreLVName  = thePreLV  -> GetName();
  G4String thePostLVName = thePostLV -> GetName();
  
  G4ThreeVector thePrePosition  = thePrePoint  -> GetPosition();
  G4ThreeVector thePostPosition = thePostPoint -> GetPosition();
  G4ThreeVector theStepPosition = 0.5 * ( thePrePosition + thePostPosition );
  
  G4Track* theTrack = theStep->GetTrack();
  G4ParticleDefinition* particleType = theTrack->GetDefinition();
  G4int trackID = theTrack->GetTrackID();
  
  G4int nStep = theTrack -> GetCurrentStepNumber();
  
  
  
  G4double global_z = theStepPosition.z()/mm + 0.5*fDetectorConstruction->GetModule_z();
  G4int coarseLayer_z = -1;
  if( global_z >= 0 )   
  coarseLayer_z = int( global_z / fDetectorConstruction->GetSpacing_z() );
  
  G4double local_z = global_z - coarseLayer_z*fDetectorConstruction->GetSpacing_z();
  G4int fineLayer_z = -1;
  if( (coarseLayer_z >= 0 ) &&
      (local_z >= 0.5*fDetectorConstruction->GetSpacing_z() - fDetectorConstruction->GetFiberRadius()) &&
      (local_z <= 0.5*fDetectorConstruction->GetSpacing_z() + fDetectorConstruction->GetFiberRadius()) )
    fineLayer_z = 0;
  if( (coarseLayer_z >= 0 ) && 
      (local_z >= 0.5*fDetectorConstruction->GetSpacing_z() + fDetectorConstruction->GetSpacing_xy() - fDetectorConstruction->GetFiberRadius()) &&
      (local_z <= 0.5*fDetectorConstruction->GetSpacing_z() + fDetectorConstruction->GetSpacing_xy() + fDetectorConstruction->GetFiberRadius()) )
    fineLayer_z = 1;
  
  G4double global_x = theStepPosition.x()/mm + 0.5*fDetectorConstruction->GetNModules_x()*fDetectorConstruction->GetModule_x();
  G4int nModule_x = int( global_x / (fDetectorConstruction->GetModule_x()) );
  G4double local_x = global_x - nModule_x*fDetectorConstruction->GetModule_x() - 0.5*(fDetectorConstruction->GetModule_x()-fDetectorConstruction->GetFiberLength());
  G4int fiberIX = nModule_x*2.*fDetectorConstruction->GetNFibers_xy() + int( local_x / (0.5*fDetectorConstruction->GetSpacing_xy()) );
  
  G4double global_y = theStepPosition.y()/mm + 0.5*fDetectorConstruction->GetNModules_y()*fDetectorConstruction->GetModule_y();
  G4int nModule_y = int( global_y / (fDetectorConstruction->GetModule_y()) );
  G4double local_y = global_y - nModule_y*fDetectorConstruction->GetModule_y() - 0.5*(fDetectorConstruction->GetModule_y()-fDetectorConstruction->GetFiberLength());
  G4int fiberIY = nModule_y*2.*fDetectorConstruction->GetNFibers_xy() + int( local_y / (0.5*fDetectorConstruction->GetSpacing_xy()) );
  
  
  
  //-----------------
  // primary particle
  if( trackID == 1 )
  {
    if( nStep-1 < 1000 )
    {
      CreateTree::Instance()->PrimaryParticleX[nStep-1] = thePrePosition.x()/mm;
      CreateTree::Instance()->PrimaryParticleY[nStep-1] = thePrePosition.y()/mm;
      CreateTree::Instance()->PrimaryParticleZ[nStep-1] = thePrePosition.z()/mm;
      CreateTree::Instance()->PrimaryParticleE[nStep-1] = thePrePoint->GetTotalEnergy()/GeV;
    }
  }
  
  
  
  //--------------
  // optical photon
  if( particleType == G4OpticalPhoton::OpticalPhotonDefinition() )
  { 
    G4String processName = theTrack->GetCreatorProcess()->GetProcessName();
    
    //if( (CreateTree::Instance()->Timing() && processName != "Cerenkov") ||
    //    (!CreateTree::Instance()->Timing()) )
    //theTrack->SetTrackStatus(fKillTrackAndSecondaries);
    
    if( (!CreateTree::Instance()->Timing()) ||
        (CreateTree::Instance()->Timing() && thePrePoint->GetGlobalTime()/picosecond > 5000.) )
      theTrack->SetTrackStatus(fKillTrackAndSecondaries);
    
    
    if( nStep == 1 && processName == "Cerenkov" ) CreateTree::Instance()->Tot_phot_cer += 1;
    
    
    // do this cycle only IF info on optical photons is needed
    if( CreateTree::Instance()->OpPhotons() )
    {
      //---------------------------------------------------------
      // storing time, energy and position of all optical photons
      
      if( (theTrack->GetLogicalVolumeAtVertex()->GetName() == "Fiber") && (nStep == 1) )
      {
        G4String processName = theTrack->GetCreatorProcess()->GetProcessName();
        if     ( processName == "Cerenkov" )      CreateTree::Instance()->opPhoton_process.push_back( +1 );
        else if( processName == "Scintillation" ) CreateTree::Instance()->opPhoton_process.push_back( +2 );
        else                                      CreateTree::Instance()->opPhoton_process.push_back( -1 );
        
        CreateTree::Instance()->opPhoton_n++;
        CreateTree::Instance()->opPhoton_trackID.push_back( trackID );
        CreateTree::Instance()->opPhoton_parentTrackID.push_back( theTrack->GetParentID() );
        if( fineLayer_z == 0 )
        {
          CreateTree::Instance()->opPhoton_fiberIX.push_back( fiberIX );
          CreateTree::Instance()->opPhoton_fiberIY.push_back( -1 );
        }
        if( fineLayer_z == 1 )
        {
          CreateTree::Instance()->opPhoton_fiberIX.push_back( -1 );
          CreateTree::Instance()->opPhoton_fiberIY.push_back( fiberIY );
        }
        CreateTree::Instance()->opPhoton_fiberIZ.push_back( 2*coarseLayer_z + fineLayer_z );
        CreateTree::Instance()->opPhoton_energy.push_back( theTrack->GetTotalEnergy()/eV );
        CreateTree::Instance()->opPhoton_waveLength.push_back( MyMaterials::fromEvToNm(theTrack->GetTotalEnergy()/eV) );
        CreateTree::Instance()->opPhoton_time.push_back( thePrePoint->GetGlobalTime()/picosecond );
        CreateTree::Instance()->opPhoton_vertexX.push_back( thePrePoint->GetPosition().x()/mm );
        CreateTree::Instance()->opPhoton_vertexY.push_back( thePrePoint->GetPosition().y()/mm );
        CreateTree::Instance()->opPhoton_vertexZ.push_back( thePrePoint->GetPosition().z()/mm );
        CreateTree::Instance()->opPhoton_pX.push_back( theTrack->GetVertexMomentumDirection().x() );
        CreateTree::Instance()->opPhoton_pY.push_back( theTrack->GetVertexMomentumDirection().y() );
        CreateTree::Instance()->opPhoton_pZ.push_back( theTrack->GetVertexMomentumDirection().z() );
      }
      
      
      //----------------------------------------------
      // storing time, energy and position at detector
      
      if( thePreLVName == "DetLayer" && thePostLVName == "Detector" )
      {
        if( theTrack->GetCreatorProcess() )
        {
          G4String processName = theTrack->GetCreatorProcess()->GetProcessName();
          if     ( processName == "Cerenkov" )      CreateTree::Instance()->opPhoton_process_det.push_back( +1 );
          else if( processName == "Scintillation" ) CreateTree::Instance()->opPhoton_process_det.push_back( +2 );
          else                                      CreateTree::Instance()->opPhoton_process_det.push_back( -1 );
        }
        
        CreateTree::Instance()->opPhoton_n_det++;
        CreateTree::Instance()->opPhoton_trackID_det.push_back( trackID );
        CreateTree::Instance()->opPhoton_parentTrackID_det.push_back( theTrack->GetParentID() );
        if( fineLayer_z == 0 )
        {
          CreateTree::Instance()->opPhoton_fiberIX_det.push_back( fiberIX );
          CreateTree::Instance()->opPhoton_fiberIY_det.push_back( -1 );
        }
        if( fineLayer_z == 1 )
        {
          CreateTree::Instance()->opPhoton_fiberIX_det.push_back( -1 );
          CreateTree::Instance()->opPhoton_fiberIY_det.push_back( fiberIY );
        }
        CreateTree::Instance()->opPhoton_fiberIZ_det.push_back( 2*coarseLayer_z + fineLayer_z );
        CreateTree::Instance()->opPhoton_energy_det.push_back( theTrack->GetTotalEnergy()/eV );
        CreateTree::Instance()->opPhoton_waveLength_det.push_back( MyMaterials::fromEvToNm(theTrack->GetTotalEnergy()/eV) );
        CreateTree::Instance()->opPhoton_time_det.push_back( theTrack->GetGlobalTime()/picosecond );
      }
    } // do this cycle only IF info on optical photons is needed
  } // optical photon
  
  
  // non optical photon
  else
  {
    float energy = theStep->GetTotalEnergyDeposit()/GeV;
    float ion_energy = theStep->GetTotalEnergyDeposit()/GeV - theStep->GetNonIonizingEnergyDeposit()/GeV;
    float nonion_energy = theStep->GetNonIonizingEnergyDeposit()/GeV;
    
    if( energy > 0 )
    {	
      // world
      {
        CreateTree::Instance()->Total_energy_world        += energy;
        CreateTree::Instance()->Total_ion_energy_world    += ion_energy;
        CreateTree::Instance()->Total_nonion_energy_world += nonion_energy;
      }
      
      // absorber
      if( thePreLVName == "Absorber" || thePreLVName == "Fiber" || thePreLVName == "Hole" )
      {
        CreateTree::Instance()->Total_energy_absorber        += energy;
        CreateTree::Instance()->Total_ion_energy_absorber    += ion_energy;
        CreateTree::Instance()->Total_nonion_energy_absorber += nonion_energy;
      }
      
      // fibers
      if( thePreLVName == "Fiber" )
      {
        CreateTree::Instance()->Total_energy_fibers        += energy;
        CreateTree::Instance()->Total_ion_energy_fibers    += ion_energy;
        CreateTree::Instance()->Total_nonion_energy_fibers += nonion_energy;
        
        if( fineLayer_z == 0 )
          CreateTree::Instance()->Total_ion_energy_fibers_iX[fiberIX] += ion_energy;
        if( fineLayer_z == 1 )
        CreateTree::Instance()->Total_ion_energy_fibers_iY[fiberIY] += ion_energy;
        CreateTree::Instance()->Total_ion_energy_fibers_iZ[2*coarseLayer_z+fineLayer_z] += ion_energy;
      }
      
      
      G4int iRadius = sqrt( pow(theStepPosition.x()/mm-CreateTree::Instance()->InitialPositionX,2) +
                            pow(theStepPosition.y()/mm-CreateTree::Instance()->InitialPositionY,2) ) / CreateTree::Instance()->Radial_stepLength;
      if( iRadius < 5000 )
        CreateTree::Instance()->Radial_ion_energy_absorber[iRadius] += ion_energy;
      
      G4int iDepth = (theStepPosition.z()/mm - CreateTree::Instance()->InitialPositionZ) / CreateTree::Instance()->Longitudinal_stepLength;
      if( iDepth < 1000 )
        CreateTree::Instance()->Longitudinal_ion_energy_absorber[iDepth] += ion_energy;
      
      
      if( CreateTree::Instance()->Deposition() )
      {
        CreateTree::Instance() -> depositionE.push_back(energy);
        CreateTree::Instance() -> depositionX.push_back(theStepPosition.x()/mm);
        CreateTree::Instance() -> depositionY.push_back(theStepPosition.y()/mm);
        CreateTree::Instance() -> depositionZ.push_back(theStepPosition.z()/mm);
        CreateTree::Instance() -> depositionCoarseLayer.push_back(coarseLayer_z);
        CreateTree::Instance() -> depositionFineLayer.push_back(fineLayer_z);
        CreateTree::Instance() -> depositionIX.push_back(fiberIX);
        CreateTree::Instance() -> depositionIY.push_back(fiberIY);
        
        if     ( thePreLVName == "World" )    CreateTree::Instance() -> depositionType.push_back(0);
        else if( thePreLVName == "Absorber" ) CreateTree::Instance() -> depositionType.push_back(1);
        else if( thePreLVName == "Fiber" )    CreateTree::Instance() -> depositionType.push_back(2);
        else                                  CreateTree::Instance() -> depositionType.push_back(-1);
      }
    }  
  } // non optical photon
}
