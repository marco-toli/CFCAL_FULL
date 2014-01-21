#include "CreateTree.hh"



CreateTree* CreateTree::fInstance = NULL;



CreateTree::CreateTree(TString name, bool init_data, bool deposition, bool opPhotons, bool timing)
{
  if( fInstance )
  {
    return;
  }
  
  this -> INIT_DATA    	= init_data;
  this -> DEPOSITION   	= deposition;
  this -> OPPHOTONS     = opPhotons;
  this -> TIMING        = timing;
  
  this -> fInstance = this;
  this -> fname     = name;
  this -> ftree     = new TTree(name,name);
  
  this->GetTree()->Branch("Event",&this->Event,"Event/I");
  
  this->GetTree()->Branch("Total_energy_fibers",            &this->Total_energy_fibers,            "Total_energy_fibers/F");
  this->GetTree()->Branch("Total_ion_energy_fibers",        &this->Total_ion_energy_fibers,        "Total_ion_energy_fibers/F");
  this->GetTree()->Branch("Total_nonion_energy_fibers",     &this->Total_nonion_energy_fibers,     "Total_nonion_energy_fibers/F");
  this->GetTree()->Branch("Total_ion_energy_fibers_att",    &this->Total_ion_energy_fibers_att,    "Total_ion_energy_fibers_att[10]/F");
  this->GetTree()->Branch("Total_ion_energy_fibers_att_top",&this->Total_ion_energy_fibers_att_top,"Total_ion_energy_fibers_att_top[10]/F");
  this->GetTree()->Branch("Total_ion_energy_fibers_att_btm",&this->Total_ion_energy_fibers_att_btm,"Total_ion_energy_fibers_att_btm[10]/F");
  this->GetTree()->Branch("Total_ion_energy_fibers_iX",     &this->Total_ion_energy_fibers_iX,     "Total_ion_energy_fibers_iX[1000]/F");
  this->GetTree()->Branch("Total_ion_energy_fibers_iY",     &this->Total_ion_energy_fibers_iY,     "Total_ion_energy_fibers_iY[1000]/F");
  this->GetTree()->Branch("Total_ion_energy_fibers_iZ",     &this->Total_ion_energy_fibers_iZ,     "Total_ion_energy_fibers_iZ[1000]/F");
  this->GetTree()->Branch("Total_ion_energy_fibers_iXiZ",     &this->Total_ion_energy_fibers_iXiZ,     "Total_ion_energy_fibers_iXiZ[1000]/F");
  this->GetTree()->Branch("Total_ion_energy_fibers_iYiZ",     &this->Total_ion_energy_fibers_iYiZ,     "Total_ion_energy_fibers_iYiZ[1000]/F");
  
  this->GetTree()->Branch("Total_energy_absorber",       &this->Total_energy_absorber,       "Total_energy_absorber/F");
  this->GetTree()->Branch("Total_ion_energy_absorber",   &this->Total_ion_energy_absorber,   "Total_ion_energy_absorber/F");
  this->GetTree()->Branch("Total_nonion_energy_absorber", &this->Total_nonion_energy_absorber,"Total_nonion_energy_absorber/F");
  
  this->GetTree()->Branch("Total_energy_preshower",       &this->Total_energy_preshower,       "Total_energy_preshower/F");
  this->GetTree()->Branch("Total_ion_energy_preshower",   &this->Total_ion_energy_preshower,   "Total_ion_energy_preshower/F");
  this->GetTree()->Branch("Total_nonion_energy_preshower", &this->Total_nonion_energy_preshower,"Total_nonion_energy_preshower/F");
  
  this->GetTree()->Branch("Total_energy_postshower",       &this->Total_energy_postshower,       "Total_energy_postshower/F");
  this->GetTree()->Branch("Total_ion_energy_postshower",   &this->Total_ion_energy_postshower,   "Total_ion_energy_postshower/F");
  this->GetTree()->Branch("Total_nonion_energy_postshower", &this->Total_nonion_energy_postshower,"Total_nonion_energy_postshower/F");
  
  this->GetTree()->Branch("Total_energy_sideshower",       &this->Total_energy_sideshower,       "Total_energy_sideshower/F");
  this->GetTree()->Branch("Total_ion_energy_sideshower",   &this->Total_ion_energy_sideshower,   "Total_ion_energy_sideshower/F");
  this->GetTree()->Branch("Total_nonion_energy_sideshower", &this->Total_nonion_energy_sideshower,"Total_nonion_energy_sideshower/F");
  
  this->GetTree()->Branch("Total_energy_world",       &this->Total_energy_world,       "Total_energy_world/F");
  this->GetTree()->Branch("Total_ion_energy_world",   &this->Total_ion_energy_world,   "Total_ion_energy_world/F");
  this->GetTree()->Branch("Total_nonion_energy_world",&this->Total_nonion_energy_world,"Total_nonion_energy_world/F");
  
  this->GetTree()->Branch("Tot_phot_cer",     &this->Tot_phot_cer,     "Tot_phot_cer/F");
  this->GetTree()->Branch("Tot_phot_cer_iX",  &this->Tot_phot_cer_iX,  "Tot_phot_cer_iX[1000]/F");
  this->GetTree()->Branch("Tot_phot_cer_iY",  &this->Tot_phot_cer_iY,  "Tot_phot_cer_iY[1000]/F");
  this->GetTree()->Branch("Tot_phot_cer_iZ",  &this->Tot_phot_cer_iZ,  "Tot_phot_cer_iZ[1000]/F");
  this->GetTree()->Branch("Tot_phot_cer_iXiZ",&this->Tot_phot_cer_iXiZ,"Tot_phot_cer_iXiZ[1000]/F");
  this->GetTree()->Branch("Tot_phot_cer_iYiZ",&this->Tot_phot_cer_iYiZ,"Tot_phot_cer_iYiZ[1000]/F");
  
  this->GetTree()->Branch("Total_em_energy",&this->Total_em_energy,"Total_em_energy/F");
  
  this->GetTree()->Branch("Radial_stepLength",               &this->Radial_stepLength,               "Radial_stepLength/F");
  this->GetTree()->Branch("Longitudinal_stepLength",         &this->Longitudinal_stepLength,         "Longitudinal_stepLength/F");
  this->GetTree()->Branch("Radial_ion_energy_absorber",      &this->Radial_ion_energy_absorber,      "Radial_ion_energy_absorber[5000]/F");
  this->GetTree()->Branch("Longitudinal_ion_energy_absorber",&this->Longitudinal_ion_energy_absorber,"Longitudinal_ion_energy_absorber[1000]/F");
  
  if( this -> INIT_DATA )
  {
    this->GetTree()->Branch("InitialEnergy",&this->InitialEnergy,"InitialEnergy/F");
    
    this->GetTree()->Branch("InitialPositionX",&this->InitialPositionX,"InitialPositionX/F");
    this->GetTree()->Branch("InitialPositionY",&this->InitialPositionY,"InitialPositionY/F");
    this->GetTree()->Branch("InitialPositionZ",&this->InitialPositionZ,"InitialPositionZ/F");
    
    this->GetTree()->Branch("InitalMomentumDirectionX",&this->InitalMomentumDirectionX,"InitalMomentumDirectionX/F");
    this->GetTree()->Branch("InitalMomentumDirectionY",&this->InitalMomentumDirectionY,"InitalMomentumDirectionY/F");
    this->GetTree()->Branch("InitalMomentumDirectionZ",&this->InitalMomentumDirectionZ,"InitalMomentumDirectionZ/F");
    
    this->GetTree()->Branch("PrimaryParticleX",&this->PrimaryParticleX,"PrimaryParticleX[1000]/F");
    this->GetTree()->Branch("PrimaryParticleY",&this->PrimaryParticleY,"PrimaryParticleY[1000]/F");
    this->GetTree()->Branch("PrimaryParticleZ",&this->PrimaryParticleZ,"PrimaryParticleZ[1000]/F");
    this->GetTree()->Branch("PrimaryParticleE",&this->PrimaryParticleE,"PrimaryParticleE[1000]/F");
  }
  
  if( this -> DEPOSITION )
  { 
    this->GetTree()->Branch("depositionE",&this->depositionE);
    this->GetTree()->Branch("depositionX",&this->depositionX);
    this->GetTree()->Branch("depositionY",&this->depositionY);
    this->GetTree()->Branch("depositionZ",&this->depositionZ);
    this->GetTree()->Branch("depositionCoarseLayer",&this->depositionCoarseLayer);
    this->GetTree()->Branch("depositionFineLayer",&this->depositionFineLayer);
    this->GetTree()->Branch("depositionIX",&this->depositionIX);
    this->GetTree()->Branch("depositionIY",&this->depositionIY);
    this->GetTree()->Branch("depositionFiberLocalX",&this->depositionFiberLocalX);
    this->GetTree()->Branch("depositionFiberLocalY",&this->depositionFiberLocalY);
    this->GetTree()->Branch("depositionFiberLocalZ",&this->depositionFiberLocalZ);
    this->GetTree()->Branch("depositionType",&this->depositionType);
  }
  
  if( this -> OPPHOTONS)
  {
    this->GetTree()->Branch("opPhoton_n",&this->opPhoton_n,"opPhoton_n/I");
    this->GetTree()->Branch("opPhoton_process",&opPhoton_process);
    this->GetTree()->Branch("opPhoton_trackID",&opPhoton_trackID);
    this->GetTree()->Branch("opPhoton_parentTrackID",&opPhoton_parentTrackID);
    this->GetTree()->Branch("opPhoton_fiberIX",&opPhoton_fiberIX);
    this->GetTree()->Branch("opPhoton_fiberIY",&opPhoton_fiberIY);
    this->GetTree()->Branch("opPhoton_fiberIZ",&opPhoton_fiberIZ);
    this->GetTree()->Branch("opPhoton_energy",&opPhoton_energy);
    this->GetTree()->Branch("opPhoton_time",&opPhoton_time);
    //this->GetTree()->Branch("opPhoton_vertexX",&opPhoton_vertexX);
    //this->GetTree()->Branch("opPhoton_vertexY",&opPhoton_vertexY);
    //this->GetTree()->Branch("opPhoton_vertexZ",&opPhoton_vertexZ);
    //this->GetTree()->Branch("opPhoton_pX",&opPhoton_pX);
    //this->GetTree()->Branch("opPhoton_pY",&opPhoton_pY);
    //this->GetTree()->Branch("opPhoton_pZ",&opPhoton_pZ);
    //this->GetTree()->Branch("opPhoton_gapProb0",&opPhoton_gapProb0);
    //this->GetTree()->Branch("opPhoton_gapTime0",&opPhoton_gapTime0);
    //this->GetTree()->Branch("opPhoton_gapProb1",&opPhoton_gapProb1);
    //this->GetTree()->Branch("opPhoton_gapTime1",&opPhoton_gapTime1);
    //this->GetTree()->Branch("opPhoton_gapProb2",&opPhoton_gapProb2);
    //this->GetTree()->Branch("opPhoton_gapTime2",&opPhoton_gapTime2);
    //this->GetTree()->Branch("opPhoton_gapProb3",&opPhoton_gapProb3);
    //this->GetTree()->Branch("opPhoton_gapTime3",&opPhoton_gapTime3);
    //this->GetTree()->Branch("opPhoton_gapProb4",&opPhoton_gapProb4);
    //this->GetTree()->Branch("opPhoton_gapTime4",&opPhoton_gapTime4);
    
    /*
    this->GetTree()->Branch("opPhotonFast_n_gap",&this->opPhotonFast_n_gap,"opPhotonFast_n_gap/I");
    this->GetTree()->Branch("opPhotonFast_process_gap",&opPhotonFast_process_gap);
    this->GetTree()->Branch("opPhotonFast_trackID_gap",&opPhotonFast_trackID_gap);
    this->GetTree()->Branch("opPhotonFast_parentTrackID_gap",&opPhotonFast_parentTrackID_gap);
    this->GetTree()->Branch("opPhotonFast_fiberIX_gap",&opPhotonFast_fiberIX_gap);
    this->GetTree()->Branch("opPhotonFast_fiberIY_gap",&opPhotonFast_fiberIY_gap);
    this->GetTree()->Branch("opPhotonFast_fiberIZ_gap",&opPhotonFast_fiberIZ_gap);
    this->GetTree()->Branch("opPhotonFast_side_gap",&opPhotonFast_side_gap);
    this->GetTree()->Branch("opPhotonFast_energy_gap",&opPhotonFast_energy_gap);
    this->GetTree()->Branch("opPhotonFast_prodTime_gap",&opPhotonFast_prodTime_gap);
    this->GetTree()->Branch("opPhotonFast_time_gap",&opPhotonFast_time_gap);
    
    this->GetTree()->Branch("opPhoton_n_gap",&this->opPhoton_n_gap,"opPhoton_n_gap/I");
    this->GetTree()->Branch("opPhoton_process_gap",&opPhoton_process_gap);
    this->GetTree()->Branch("opPhoton_trackID_gap",&opPhoton_trackID_gap);
    this->GetTree()->Branch("opPhoton_parentTrackID_gap",&opPhoton_parentTrackID_gap);
    this->GetTree()->Branch("opPhoton_fiberIX_gap",&opPhoton_fiberIX_gap);
    this->GetTree()->Branch("opPhoton_fiberIY_gap",&opPhoton_fiberIY_gap);
    this->GetTree()->Branch("opPhoton_fiberIZ_gap",&opPhoton_fiberIZ_gap);
    this->GetTree()->Branch("opPhoton_side_gap",&opPhoton_side_gap);
    this->GetTree()->Branch("opPhoton_energy_gap",&opPhoton_energy_gap);
    this->GetTree()->Branch("opPhoton_prodTime_gap",&opPhoton_prodTime_gap);
    this->GetTree()->Branch("opPhoton_time_gap",&opPhoton_time_gap);
    */
    
    this->GetTree()->Branch("opPhoton_n_det",&this->opPhoton_n_det,"opPhoton_n_det/I");
    this->GetTree()->Branch("opPhoton_process_det",&opPhoton_process_det);
    this->GetTree()->Branch("opPhoton_trackID_det",&opPhoton_trackID_det);
    this->GetTree()->Branch("opPhoton_parentTrackID_det",&opPhoton_parentTrackID_det);
    this->GetTree()->Branch("opPhoton_fiberIX_det",&opPhoton_fiberIX_det);
    this->GetTree()->Branch("opPhoton_fiberIY_det",&opPhoton_fiberIY_det);
    this->GetTree()->Branch("opPhoton_fiberIZ_det",&opPhoton_fiberIZ_det);
    this->GetTree()->Branch("opPhoton_side_det",&opPhoton_side_det);
    this->GetTree()->Branch("opPhoton_energy_det",&opPhoton_energy_det);
    this->GetTree()->Branch("opPhoton_prodTime_det",&opPhoton_prodTime_det);
    this->GetTree()->Branch("opPhoton_time_det",&opPhoton_time_det);
  }
  
  this->Clear();
}



CreateTree::~CreateTree()
{}



bool CreateTree::Write()
{
  TString filename = this->GetName();
  filename+=".root";
  TFile* file = new TFile(filename,"RECREATE");
  this->GetTree()->Write();
  file->Write();
  file->Close();
  return true;
}



void CreateTree::Clear()
{
  Event	= 0;
  
  
  Total_energy_fibers        = 0;
  Total_ion_energy_fibers    = 0;
  Total_nonion_energy_fibers = 0;
  for(int i = 0; i < 10; ++i)
  {
    Total_ion_energy_fibers_att[i] = 0;
    Total_ion_energy_fibers_att_top[i] = 0;
    Total_ion_energy_fibers_att_btm[i] = 0;
  }
  for(int i = 0; i < 1000; ++i)
  {
    Total_ion_energy_fibers_iX[i] = 0;
    Total_ion_energy_fibers_iY[i] = 0;
    Total_ion_energy_fibers_iZ[i] = 0;
    Total_ion_energy_fibers_iXiZ[i] = 0;
    Total_ion_energy_fibers_iYiZ[i] = 0;
  }
  
  Total_energy_absorber        = 0;
  Total_ion_energy_absorber    = 0;
  Total_nonion_energy_absorber = 0;
  
  Total_energy_preshower        = 0;
  Total_ion_energy_preshower    = 0;
  Total_nonion_energy_preshower = 0;
  
  Total_energy_postshower        = 0;
  Total_ion_energy_postshower    = 0;
  Total_nonion_energy_postshower = 0;
  
  Total_energy_sideshower        = 0;
  Total_ion_energy_sideshower    = 0;
  Total_nonion_energy_sideshower = 0;
  
  Total_energy_world        = 0;
  Total_ion_energy_world    = 0;
  Total_nonion_energy_world = 0;
  
  Tot_phot_cer = 0;
  for(int i = 0; i < 1000; ++i)
  {
    Tot_phot_cer_iX[i] = 0;
    Tot_phot_cer_iY[i] = 0;
    Tot_phot_cer_iZ[i] = 0;
    Tot_phot_cer_iXiZ[i] = 0;
    Tot_phot_cer_iYiZ[i] = 0;
  }
  
  Total_em_energy = 0;
  
  Radial_stepLength = 0.;
  Longitudinal_stepLength = 0.;
  for(int i = 0; i < 5000; ++i)
    Radial_ion_energy_absorber[i] = 0.;
  for(int i = 0; i < 1000; ++i)
    Longitudinal_ion_energy_absorber[i] = 0.;
  
  
  if( this->INIT_DATA )
  {
    InitialEnergy = 0;
    
    InitialPositionX = 0;
    InitialPositionY = 0;
    InitialPositionZ = 0;
    
    InitalMomentumDirectionX = 0;
    InitalMomentumDirectionY = 0;
    InitalMomentumDirectionZ = 0;
    
    for(int i = 0; i < 1000; ++i)
    {
      PrimaryParticleX[i] = 0.;
      PrimaryParticleY[i] = 0.;
      PrimaryParticleZ[i] = 0.;
      PrimaryParticleE[i] = 0.;
    }
  }
  
  
  if( this->DEPOSITION )
  {
    depositionE.clear();
    depositionX.clear();
    depositionY.clear();		
    depositionZ.clear();
    depositionCoarseLayer.clear();
    depositionFineLayer.clear();
    depositionIX.clear();
    depositionIY.clear();
    depositionFiberLocalX.clear();
    depositionFiberLocalY.clear();
    depositionFiberLocalZ.clear();
    depositionType.clear();
  }
  
  
  if( this->OPPHOTONS )
  {
    opPhoton_n = 0;
    opPhoton_process.clear();
    opPhoton_trackID.clear();
    opPhoton_parentTrackID.clear();
    opPhoton_fiberIX.clear();
    opPhoton_fiberIY.clear();
    opPhoton_fiberIZ.clear();
    opPhoton_energy.clear();
    opPhoton_time.clear();
    //opPhoton_vertexX.clear();
    //opPhoton_vertexY.clear();
    //opPhoton_vertexZ.clear();
    //opPhoton_pX.clear();
    //opPhoton_pY.clear();
    //opPhoton_pZ.clear();
    //opPhoton_gapProb0.clear();
    //opPhoton_gapTime0.clear();
    //opPhoton_gapProb1.clear();
    //opPhoton_gapTime1.clear();
    //opPhoton_gapProb2.clear();
    //opPhoton_gapTime2.clear();
    //opPhoton_gapProb3.clear();
    //opPhoton_gapTime3.clear();
    //opPhoton_gapProb4.clear();
    //opPhoton_gapTime4.clear();
    
    /*
    opPhotonFast_n_gap = 0;
    opPhotonFast_process_gap.clear();
    opPhotonFast_trackID_gap.clear();
    opPhotonFast_parentTrackID_gap.clear();
    opPhotonFast_fiberIX_gap.clear();
    opPhotonFast_fiberIY_gap.clear();
    opPhotonFast_fiberIZ_gap.clear();
    opPhotonFast_side_gap.clear();
    opPhotonFast_energy_gap.clear();
    opPhotonFast_prodTime_gap.clear();
    opPhotonFast_time_gap.clear();
    
    opPhoton_n_gap = 0;
    opPhoton_process_gap.clear();
    opPhoton_trackID_gap.clear();
    opPhoton_parentTrackID_gap.clear();
    opPhoton_fiberIX_gap.clear();
    opPhoton_fiberIY_gap.clear();
    opPhoton_fiberIZ_gap.clear();
    opPhoton_side_gap.clear();
    opPhoton_energy_gap.clear();
    opPhoton_prodTime_gap.clear();
    opPhoton_time_gap.clear();
    */
    
    opPhoton_n_det = 0;
    opPhoton_process_det.clear();
    opPhoton_trackID_det.clear();
    opPhoton_parentTrackID_det.clear();
    opPhoton_fiberIX_det.clear();
    opPhoton_fiberIY_det.clear();
    opPhoton_fiberIZ_det.clear();
    opPhoton_side_det.clear();
    opPhoton_energy_det.clear();
    opPhoton_prodTime_det.clear();
    opPhoton_time_det.clear();
  }
}
