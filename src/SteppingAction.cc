#include "SteppingAction.hh"



SteppingAction::SteppingAction(const DetectorConstruction* detectorConstruction, Fiber* vFib, Fiber* hFib):
  fDetectorConstruction(detectorConstruction),
  fVFiber(vFib),
  fHFiber(hFib)
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
  
  G4String thePrePVName  = thePrePV  -> GetName();
  G4String thePostPVName = thePostPV -> GetName();  
  G4LogicalVolume* thePreLV  = thePrePV  -> GetLogicalVolume();
  G4LogicalVolume* thePostLV = thePostPV -> GetLogicalVolume();
  G4String thePreLVName  = thePreLV  -> GetName();
  G4String thePostLVName = thePostLV -> GetName();
  
  G4ThreeVector thePrePosition  = thePrePoint  -> GetPosition();
  
  G4Track* theTrack = theStep->GetTrack();
  G4ParticleDefinition* particleType = theTrack->GetDefinition();
  G4int trackID = theTrack->GetTrackID();
  TrackInformation* theTrackInfo = (TrackInformation*)(theTrack->GetUserInformation());
  
  G4int nStep = theTrack -> GetCurrentStepNumber();
  
  
  
  G4double hole_radius  = fDetectorConstruction->GetHoleRadius();
  G4double fiber_radius = fDetectorConstruction->GetFiberRadius();
  G4double fiber_length = fDetectorConstruction->GetFiberLength();
  G4double spacing_x = fDetectorConstruction->GetSpacing_x();
  G4double spacing_y = fDetectorConstruction->GetSpacing_y();
  G4double spacing_z = fDetectorConstruction->GetSpacing_z();
  G4double offset_z = fDetectorConstruction->GetOffset_z();
  
  
  G4double point_x = thePrePosition.x()/mm;
  G4double point_y = thePrePosition.y()/mm;
  G4double point_z = thePrePosition.z()/mm;
  if( thePreLVName == "Gap" || thePreLVName == "GapLayer" || thePreLVName == "Detector" || thePreLVName == "DetLayer" )
  {
    point_x = (theTrack->GetVertexPosition()).x()/mm;
    point_y = (theTrack->GetVertexPosition()).y()/mm;
    point_z = (theTrack->GetVertexPosition()).z()/mm;
  }
  
  
  G4double global_z = point_z + 0.5*fDetectorConstruction->GetModule_z();
  G4int coarseLayer_z = -1;
  if( global_z >= 0 )
    coarseLayer_z = int( global_z / spacing_z );
  
  G4double local_z = global_z - coarseLayer_z*spacing_z;
  G4int fineLayer_z = -1;
  if( (coarseLayer_z >= 0 ) &&
      (local_z >= 0.5*spacing_z - fiber_radius - offset_z) &&
      (local_z <= 0.5*spacing_z + fiber_radius - offset_z) )
    fineLayer_z = 0;
  if( (coarseLayer_z >= 0 ) &&
      (local_z >= 0.5*spacing_z - fiber_radius + offset_z) &&
      (local_z <= 0.5*spacing_z + fiber_radius + offset_z) )
    fineLayer_z = 1;
  
  G4double global_x = point_x + 0.5*fDetectorConstruction->GetNModules_x()*fDetectorConstruction->GetModule_x();
  G4int nModule_x = int( global_x / (fDetectorConstruction->GetModule_x()) );
  G4double local_x = global_x - nModule_x*fDetectorConstruction->GetModule_x() - 0.5*(fDetectorConstruction->GetModule_x()-fDetectorConstruction->GetActiveArea_x());
  
  G4double global_y = point_y + 0.5*fDetectorConstruction->GetNModules_y()*fDetectorConstruction->GetModule_y();
  G4int nModule_y = int( global_y / (fDetectorConstruction->GetModule_y()) );
  G4double local_y = global_y - nModule_y*fDetectorConstruction->GetModule_y() - 0.5*(fDetectorConstruction->GetModule_y()-fDetectorConstruction->GetActiveArea_y());
  
  G4int fiberIX = -999;
  G4int fiberIY = -999;
  G4double fiberLocal_x = -999.;
  G4double fiberLocal_y = -999.;
  G4double fiberLocal_z = -999.;
  
  if( fineLayer_z == 0 )
  {
    fiberLocal_x = local_x - 0.5*spacing_x*int( local_x / (0.5*spacing_x) ) - hole_radius;
    fiberLocal_y = local_y - 0.5*fiber_length;
    fiberLocal_z = local_z - ( 0.5*spacing_z - 2. );
    
    fiberIX = nModule_x*2.*fDetectorConstruction->GetNFibers_x() + int( local_x / (0.5*spacing_x) );
    fiberIY = nModule_y;
  }
  if( fineLayer_z == 1 )
  {
    fiberLocal_x = local_x - 0.5*fiber_length;
    fiberLocal_y = local_y - 0.5*spacing_y*int( local_y / (0.5*spacing_y) ) - hole_radius;
    fiberLocal_z = local_z - ( 0.5*spacing_z + 2. );
    
    fiberIX = nModule_x;
    fiberIY = nModule_y*2.*fDetectorConstruction->GetNFibers_y() + int( local_y / (0.5*spacing_y) );
  }
  
  
  
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
  
  
  
  //if( (trackID != 1) && (particleType == G4Gamma::GammaDefinition()) )
  //{ 
  //  if( thePostLVName == "World" )
  //    theTrack->SetTrackStatus(fKillTrackAndSecondaries);      
  //}
  
  
  
  //--------------
  // optical photon
  if( particleType == G4OpticalPhoton::OpticalPhotonDefinition() )
  { 
    G4String processName = theTrack->GetCreatorProcess()->GetProcessName();
    
    if( (CreateTree::Instance()->Timing() && processName != "Cerenkov") ||
        (!CreateTree::Instance()->Timing()) )
      theTrack->SetTrackStatus(fKillTrackAndSecondaries);
    
    //theTrack->SetTrackStatus(fKillTrackAndSecondaries);
    
    //if( (!CreateTree::Instance()->Timing()) ||
    //    (CreateTree::Instance()->Timing() && thePrePoint->GetGlobalTime()/picosecond > 5000.) )
    //  theTrack->SetTrackStatus(fKillTrackAndSecondaries);
    
    
    if( (theTrack->GetLogicalVolumeAtVertex()->GetName() == "Fiber") && (nStep == 1) && (processName == "Cerenkov") )
    CreateTree::Instance()->Tot_phot_cer += 1;
    
    
    // do this cycle only IF info on optical photons is needed
    if( CreateTree::Instance()->OpPhotons() )
    {
      //---------------------------------------------------------
      // storing time, energy and position of all optical photons
      
      if( (theTrack->GetLogicalVolumeAtVertex()->GetName() == "Fiber") && (nStep == 1) )
      {
        if     ( processName == "Cerenkov" )      CreateTree::Instance()->opPhoton_process.push_back( +1 );
        else if( processName == "Scintillation" ) CreateTree::Instance()->opPhoton_process.push_back( +2 );
        else                                      CreateTree::Instance()->opPhoton_process.push_back( -1 );
        
        CreateTree::Instance()->opPhoton_n++;
        CreateTree::Instance()->opPhoton_trackID.push_back( trackID );
        CreateTree::Instance()->opPhoton_parentTrackID.push_back( theTrack->GetParentID() );
        CreateTree::Instance()->opPhoton_fiberIX.push_back( fiberIX );
        CreateTree::Instance()->opPhoton_fiberIY.push_back( fiberIY );
        CreateTree::Instance()->opPhoton_fiberIZ.push_back( 2*coarseLayer_z + fineLayer_z );
        CreateTree::Instance()->opPhoton_energy.push_back( theTrack->GetTotalEnergy()/eV );
        CreateTree::Instance()->opPhoton_prodTime.push_back( theTrackInfo->GetParticleProdTime() );
        CreateTree::Instance()->opPhoton_time.push_back( thePrePoint->GetGlobalTime()/picosecond );
        //CreateTree::Instance()->opPhoton_vertexX.push_back( thePrePoint->GetPosition().x()/mm );
        //CreateTree::Instance()->opPhoton_vertexY.push_back( thePrePoint->GetPosition().y()/mm );
        //CreateTree::Instance()->opPhoton_vertexZ.push_back( thePrePoint->GetPosition().z()/mm );
        //CreateTree::Instance()->opPhoton_pX.push_back( theTrack->GetVertexMomentumDirection().x() );
        //CreateTree::Instance()->opPhoton_pY.push_back( theTrack->GetVertexMomentumDirection().y() );
        //CreateTree::Instance()->opPhoton_pZ.push_back( theTrack->GetVertexMomentumDirection().z() );
        
        //----------------------------------------------------------
        // storing time, energy and position at gap with fast timing
        Photon ph;
        ph.position.SetX(fiberLocal_x);
        ph.position.SetY(fiberLocal_y);
        ph.position.SetZ(fiberLocal_z);
        ph.direction.SetX(theTrack->GetVertexMomentumDirection().x());
        ph.direction.SetY(theTrack->GetVertexMomentumDirection().y());
        ph.direction.SetZ(theTrack->GetVertexMomentumDirection().z());
        if( fineLayer_z == 0 ) ph.dist = (fiberLocal_y+0.5*fDetectorConstruction->GetFiberLength())/fDetectorConstruction->GetFiberLength();
        if( fineLayer_z == 1 ) ph.dist = (fiberLocal_x+0.5*fDetectorConstruction->GetFiberLength())/fDetectorConstruction->GetFiberLength();
        
        Travel trc;
        if( fineLayer_z == 0 ) trc = GetTimeAndProbability(ph,fVFiber,theTrackInfo->GetParticleProdTime());
        if( fineLayer_z == 1 ) trc = GetTimeAndProbability(ph,fHFiber,theTrackInfo->GetParticleProdTime());
        
        /*
        CreateTree::Instance()->opPhoton_gapProb0.push_back( trc.prob[0] );
        CreateTree::Instance()->opPhoton_gapTime0.push_back( trc.time[0] );
        CreateTree::Instance()->opPhoton_gapProb1.push_back( trc.prob[1] );
        CreateTree::Instance()->opPhoton_gapTime1.push_back( trc.time[1] );
        CreateTree::Instance()->opPhoton_gapProb2.push_back( trc.prob[2] );
        CreateTree::Instance()->opPhoton_gapTime2.push_back( trc.time[2] );
        CreateTree::Instance()->opPhoton_gapProb3.push_back( trc.prob[3] );
        CreateTree::Instance()->opPhoton_gapTime3.push_back( trc.time[3] );
        CreateTree::Instance()->opPhoton_gapProb4.push_back( trc.prob[4] );
        CreateTree::Instance()->opPhoton_gapTime4.push_back( trc.time[4] );
        */
        
        /*
        for(int iTrial = 0; iTrial < 5; ++iTrial)
        {
          if( trc.time[iTrial] > 5000. ) break;
          double val = ran.Uniform(0.,1.);
          if( val > trc.prob[iTrial] ) continue;
          
          if     ( processName == "Cerenkov" )      CreateTree::Instance()->opPhotonFast_process_gap.push_back( +1 );
          else if( processName == "Scintillation" ) CreateTree::Instance()->opPhotonFast_process_gap.push_back( +2 );
          else                                      CreateTree::Instance()->opPhotonFast_process_gap.push_back( -1 );
          
          CreateTree::Instance()->opPhotonFast_n_gap++;
          CreateTree::Instance()->opPhotonFast_trackID_gap.push_back( trackID );
          CreateTree::Instance()->opPhotonFast_parentTrackID_gap.push_back( theTrack->GetParentID() );
          CreateTree::Instance()->opPhotonFast_fiberIX_gap.push_back( fiberIX );
          CreateTree::Instance()->opPhotonFast_fiberIY_gap.push_back( fiberIY );
          CreateTree::Instance()->opPhotonFast_fiberIZ_gap.push_back( 2*coarseLayer_z + fineLayer_z );
          CreateTree::Instance()->opPhotonFast_energy_gap.push_back( theTrack->GetTotalEnergy()/eV );
          CreateTree::Instance()->opPhotonFast_prodTime_gap.push_back( theTrackInfo->GetParticleProdTime() );
          CreateTree::Instance()->opPhotonFast_time_gap.push_back( trc.time[iTrial] );
        }
        */
        
        if( trc.prob[0] < 1.E-09 ) theTrack->SetTrackStatus(fKillTrackAndSecondaries);
      }
      
      
      /*
      //-----------------------------------------
      // storing time, energy and position at gap
      
      if( (theTrack->GetLogicalVolumeAtVertex()->GetName() == "Fiber") && (thePreLVName == "GapLayer") && (thePostLVName == "Gap") )
      {
        if     ( processName == "Cerenkov" )      CreateTree::Instance()->opPhoton_process_gap.push_back( +1 );
        else if( processName == "Scintillation" ) CreateTree::Instance()->opPhoton_process_gap.push_back( +2 );
        else                                      CreateTree::Instance()->opPhoton_process_gap.push_back( -1 );
        
        CreateTree::Instance()->opPhoton_n_gap++;
        CreateTree::Instance()->opPhoton_trackID_gap.push_back( trackID );
        CreateTree::Instance()->opPhoton_parentTrackID_gap.push_back( theTrack->GetParentID() );
        CreateTree::Instance()->opPhoton_fiberIX_gap.push_back( fiberIX );
        CreateTree::Instance()->opPhoton_fiberIY_gap.push_back( fiberIY );
        CreateTree::Instance()->opPhoton_fiberIZ_gap.push_back( 2*coarseLayer_z + fineLayer_z );
        if( thePostPVName == "vTopGap1" || thePostPVName == "vTopGap2" || 
            thePostPVName == "hTopGap1" || thePostPVName == "hTopGap2" )
          CreateTree::Instance()->opPhoton_side_gap.push_back( +1 );
        if( thePostPVName == "vBtmGap1" || thePostPVName == "vBtmGap2" || 
            thePostPVName == "hBtmGap1" || thePostPVName == "hBtmGap2" )
          CreateTree::Instance()->opPhoton_side_gap.push_back( -1 );
        CreateTree::Instance()->opPhoton_energy_gap.push_back( theTrack->GetTotalEnergy()/eV );
        CreateTree::Instance()->opPhoton_prodTime_gap.push_back( theTrackInfo->GetParticleProdTime() );
        CreateTree::Instance()->opPhoton_time_gap.push_back( theTrack->GetGlobalTime()/picosecond );
        
        //theTrack->SetTrackStatus(fKillTrackAndSecondaries);
      }
      */
      
      
      //----------------------------------------------
      // storing time, energy and position at detector
      
      if( (theTrack->GetLogicalVolumeAtVertex()->GetName() == "Fiber") && (thePreLVName == "DetLayer") && (thePostLVName == "Detector") )
      {
        if     ( processName == "Cerenkov" )      CreateTree::Instance()->opPhoton_process_det.push_back( +1 );
        else if( processName == "Scintillation" ) CreateTree::Instance()->opPhoton_process_det.push_back( +2 );
        else                                      CreateTree::Instance()->opPhoton_process_det.push_back( -1 );
        
        CreateTree::Instance()->opPhoton_n_det++;
        CreateTree::Instance()->opPhoton_trackID_det.push_back( trackID );
        CreateTree::Instance()->opPhoton_parentTrackID_det.push_back( theTrack->GetParentID() );
        CreateTree::Instance()->opPhoton_fiberIX_det.push_back( fiberIX );
        CreateTree::Instance()->opPhoton_fiberIY_det.push_back( fiberIY );
        CreateTree::Instance()->opPhoton_fiberIZ_det.push_back( 2*coarseLayer_z + fineLayer_z );
        if( thePostPVName == "vTopDetector1" || thePostPVName == "vTopDetector2" || 
            thePostPVName == "hTopDetector1" || thePostPVName == "hTopDetector2" )
          CreateTree::Instance()->opPhoton_side_det.push_back( +1 );
        if( thePostPVName == "vBtmDetector1" || thePostPVName == "vBtmDetector2" || 
            thePostPVName == "hBtmDetector1" || thePostPVName == "hBtmDetector2" )
          CreateTree::Instance()->opPhoton_side_det.push_back( -1 );
        CreateTree::Instance()->opPhoton_energy_det.push_back( theTrack->GetTotalEnergy()/eV );
        CreateTree::Instance()->opPhoton_prodTime_det.push_back( theTrackInfo->GetParticleProdTime() );
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
      
      // preshower
      if( thePreLVName == "Preshower" )
      {
        CreateTree::Instance()->Total_energy_preshower        += energy;
        CreateTree::Instance()->Total_ion_energy_preshower    += ion_energy;
        CreateTree::Instance()->Total_nonion_energy_preshower += nonion_energy;
      }
      
      // postshower
      if( thePreLVName == "Postshower" )
      {
        CreateTree::Instance()->Total_energy_postshower        += energy;
        CreateTree::Instance()->Total_ion_energy_postshower    += ion_energy;
        CreateTree::Instance()->Total_nonion_energy_postshower += nonion_energy;
      }
      
      // sideshower
      if( thePreLVName == "Sideshower" )
      {
        CreateTree::Instance()->Total_energy_sideshower        += energy;
        CreateTree::Instance()->Total_ion_energy_sideshower    += ion_energy;
        CreateTree::Instance()->Total_nonion_energy_sideshower += nonion_energy;
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
        
        for(int i = 0; i < 9; ++i)
        {
          float attLambda = fDetectorConstruction->GetAttLambda(i);
          
          if( fineLayer_z == 0 )
          {
            CreateTree::Instance()->Total_ion_energy_fibers_att[i] += ( 0.5*ion_energy*exp(-1.*local_y/attLambda) +
                                                                        0.5*ion_energy*exp(-1.*(fiber_length-local_y)/attLambda) );
            CreateTree::Instance()->Total_ion_energy_fibers_att_top[i] += ( 0.5*ion_energy*exp(-1.*(fiber_length-local_y)/attLambda) );
            CreateTree::Instance()->Total_ion_energy_fibers_att_btm[i] += ( 0.5*ion_energy*exp(-1.*local_y/attLambda) );
          }
          if( fineLayer_z == 1 )
          {
            CreateTree::Instance()->Total_ion_energy_fibers_att[i] += ( 0.5*ion_energy*exp(-1.*local_x/attLambda) +
                                                                        0.5*ion_energy*exp(-1.*(fiber_length-local_x)/attLambda) );
            CreateTree::Instance()->Total_ion_energy_fibers_att_top[i] += ( 0.5*ion_energy*exp(-1.*(fiber_length-local_x)/attLambda) );
            CreateTree::Instance()->Total_ion_energy_fibers_att_btm[i] += ( 0.5*ion_energy*exp(-1.*local_x/attLambda) );
          }
        }
      }
      
      // what else?
      if( thePreLVName != "Absorber" && 
          thePreLVName != "Fiber" && 
          thePreLVName != "Hole" && 
          thePreLVName != "Preshower" && 
          thePreLVName != "Postshower" && 
          thePreLVName != "Sideshower" && 
          thePreLVName != "World" )
      {
        std::cout << ">>> ATTENZIONE: thePreLVName: " << thePreLVName << "!!!" << std::endl;
      }
      
      
      G4int iRadius = sqrt( pow(thePrePosition.x()/mm-CreateTree::Instance()->InitialPositionX,2) +
                            pow(thePrePosition.y()/mm-CreateTree::Instance()->InitialPositionY,2) ) / CreateTree::Instance()->Radial_stepLength;
      if( iRadius < 5000 )
        CreateTree::Instance()->Radial_ion_energy_absorber[iRadius] += ion_energy;
      
      G4int iDepth = (thePrePosition.z()/mm - CreateTree::Instance()->InitialPositionZ) / CreateTree::Instance()->Longitudinal_stepLength;
      if( iDepth < 1000 )
        CreateTree::Instance()->Longitudinal_ion_energy_absorber[iDepth] += ion_energy;
      
      
      if( CreateTree::Instance()->Deposition() )
      {
        CreateTree::Instance() -> depositionE.push_back(energy);
        CreateTree::Instance() -> depositionX.push_back(thePrePosition.x()/mm);
        CreateTree::Instance() -> depositionY.push_back(thePrePosition.y()/mm);
        CreateTree::Instance() -> depositionZ.push_back(thePrePosition.z()/mm);
        CreateTree::Instance() -> depositionCoarseLayer.push_back(coarseLayer_z);
        CreateTree::Instance() -> depositionFineLayer.push_back(fineLayer_z);
        CreateTree::Instance() -> depositionIX.push_back(fiberIX);
        CreateTree::Instance() -> depositionIY.push_back(fiberIY);
        CreateTree::Instance() -> depositionFiberLocalX.push_back(fiberLocal_x);
        CreateTree::Instance() -> depositionFiberLocalY.push_back(fiberLocal_y);
        CreateTree::Instance() -> depositionFiberLocalZ.push_back(fiberLocal_z);
        
        if     ( thePreLVName == "World" )    CreateTree::Instance() -> depositionType.push_back(0);
        else if( thePreLVName == "Absorber" ) CreateTree::Instance() -> depositionType.push_back(1);
        else if( thePreLVName == "Fiber" )    CreateTree::Instance() -> depositionType.push_back(2);
        else                                  CreateTree::Instance() -> depositionType.push_back(-1);
      }
    }  
  } // non optical photon
  
}
