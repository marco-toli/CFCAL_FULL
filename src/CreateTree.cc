
#include "CreateTree.hh"



CreateTree* CreateTree::fInstance = NULL;



CreateTree::CreateTree(TString name, bool energy_fiber, bool init_data, bool pos_fiber, bool timing)
{
  if( fInstance )
  {
    return;
  }
  
  this -> ENERGY_FIBER 	= energy_fiber;
  this -> INIT_DATA    	= init_data;
  this -> POS_FIBER    	= pos_fiber;
  this -> TIMING        = timing;
  
  this -> fInstance = this;
  this -> fname     = name;
  this -> ftree     = new TTree(name,name);
  
  this->GetTree()->Branch("Event",&this->Event,"Event/I");
  
  this->GetTree()->Branch("Total_energy_fibers",&this->Total_energy_fibers,"Total_energy_fibers/F");
  this->GetTree()->Branch("Total_nonion_energy_fibers",&this->Total_nonion_energy_fibers,"Total_nonion_energy_fibers/F");
  this->GetTree()->Branch("Total_energy_absorber",&this->Total_energy_absorber,"Total_energy_absorber/F");
  this->GetTree()->Branch("Total_energy_world",&this->Total_energy_world,"Total_energy_world/F");
  
  if( this -> INIT_DATA )
  {
    this->GetTree()->Branch("InitialPositionX",&this->InitialPositionX,"InitialPositionX/F");
    this->GetTree()->Branch("InitialPositionY",&this->InitialPositionY,"InitialPositionY/F");
    this->GetTree()->Branch("InitialPositionZ",&this->InitialPositionZ,"InitialPositionZ/F");
    
    this->GetTree()->Branch("InitalMomentumDirectionX",&this->InitalMomentumDirectionX,"InitalMomentumDirectionX/F");
    this->GetTree()->Branch("InitalMomentumDirectionY",&this->InitalMomentumDirectionY,"InitalMomentumDirectionY/F");
    this->GetTree()->Branch("InitalMomentumDirectionZ",&this->InitalMomentumDirectionZ,"InitalMomentumDirectionZ/F");
  }
  
  if( this -> POS_FIBER)
  { 
    this->GetTree()->Branch("depositionX",&this->depositionX);
    this->GetTree()->Branch("depositionY",&this->depositionY);
    this->GetTree()->Branch("depositionZ",&this->depositionZ);  
    this->GetTree()->Branch("Energy_deposited",&this->Energy_deposited);
  }
  
  if( this -> ENERGY_FIBER)
  {
    this->GetTree()->Branch("Total_energy",&this->Total_energy,"Total_energy[250][300]/F");
    this->GetTree()->Branch("Total_nonion_energy",&this->Total_nonion_energy,"Total_nonion_energy[250][300]/F");	  
  }
  
  if( this -> TIMING)
  {
    this->GetTree()->Branch("opPhoton_n",&this->opPhoton_n,"opPhoton_n/I");
    this->GetTree()->Branch("opPhoton_process",&opPhoton_process);
    this->GetTree()->Branch("opPhoton_trackID",&opPhoton_trackID);
    this->GetTree()->Branch("opPhoton_parentTrackID",&opPhoton_parentTrackID);
    this->GetTree()->Branch("opPhoton_fiberIX",&opPhoton_fiberIX);
    this->GetTree()->Branch("opPhoton_fiberIY",&opPhoton_fiberIY);
    this->GetTree()->Branch("opPhoton_energy",&opPhoton_energy);
    this->GetTree()->Branch("opPhoton_waveLength",&opPhoton_waveLength);
    this->GetTree()->Branch("opPhoton_time",&opPhoton_time);
    this->GetTree()->Branch("opPhoton_vertexX",&opPhoton_vertexX);
    this->GetTree()->Branch("opPhoton_vertexY",&opPhoton_vertexY);
    this->GetTree()->Branch("opPhoton_vertexZ",&opPhoton_vertexZ);
    this->GetTree()->Branch("opPhoton_pX",&opPhoton_pX);
    this->GetTree()->Branch("opPhoton_pY",&opPhoton_pY);
    this->GetTree()->Branch("opPhoton_pZ",&opPhoton_pZ);
    
    this->GetTree()->Branch("opPhoton_n_det",&this->opPhoton_n_det,"opPhoton_n_det/I");
    this->GetTree()->Branch("opPhoton_process_det",&opPhoton_process_det);
    this->GetTree()->Branch("opPhoton_trackID_det",&opPhoton_trackID_det);
    this->GetTree()->Branch("opPhoton_parentTrackID_det",&opPhoton_parentTrackID_det);
    this->GetTree()->Branch("opPhoton_fiberIX_det",&opPhoton_fiberIX_det);
    this->GetTree()->Branch("opPhoton_fiberIY_det",&opPhoton_fiberIY_det);
    this->GetTree()->Branch("opPhoton_energy_det",&opPhoton_energy_det);
    this->GetTree()->Branch("opPhoton_waveLength_det",&opPhoton_waveLength_det);
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
  Total_nonion_energy_fibers = 0;
  Total_energy_absorber      = 0;
  Total_energy_world         = 0;
  
  if( this->INIT_DATA )
  {
    InitialPositionX = 0;
    InitialPositionY = 0;
    InitialPositionZ = 0;
    
    InitalMomentumDirectionX = 0;
    InitalMomentumDirectionY = 0;
    InitalMomentumDirectionZ = 0;
  }
  
  if( this->ENERGY_FIBER )
  {
    for (int iF_X = 0; iF_X < 250; iF_X++)
    {
      for (int iF_Y = 0; iF_Y < 300; iF_Y++)
      {
        Total_energy[iF_X][iF_Y] = 0;
        Total_nonion_energy[iF_X][iF_Y] = 0;	  		    	  
      }
    }
  }
  
  if( this->POS_FIBER )
  {
    depositionX.clear();		
    depositionY.clear();		
    depositionZ.clear();
    Energy_deposited.clear();
  }
  
  if( this->TIMING )
  {
    opPhoton_n = 0;
    opPhoton_process.clear();
    opPhoton_trackID.clear();
    opPhoton_parentTrackID.clear();
    opPhoton_fiberIX.clear();
    opPhoton_fiberIY.clear();
    opPhoton_energy.clear();
    opPhoton_waveLength.clear();
    opPhoton_time.clear();
    opPhoton_vertexX.clear();
    opPhoton_vertexY.clear();
    opPhoton_vertexZ.clear();
    opPhoton_pX.clear();
    opPhoton_pY.clear();
    opPhoton_pZ.clear();
    
    opPhoton_n_det = 0;
    opPhoton_process_det.clear();
    opPhoton_trackID_det.clear();
    opPhoton_parentTrackID_det.clear();
    opPhoton_fiberIX_det.clear();
    opPhoton_fiberIY_det.clear();
    opPhoton_energy_det.clear();
    opPhoton_waveLength_det.clear();
    opPhoton_time_det.clear();
  }
}
