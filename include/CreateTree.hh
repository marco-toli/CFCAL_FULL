#ifndef CreateTree_H
#define CreateTree_H 1

#include <iostream>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"



class CreateTree
{
private:
  
  TTree*  ftree;
  TString fname;
  
public:
  
  bool INIT_DATA; 
  bool DEPOSITION;
  bool OPPHOTONS;
  bool TIMING;
  
public:
  
  CreateTree(TString name, bool init_data, bool deposition, bool opPhotons, bool timing);
  ~CreateTree();
  
  TTree*             GetTree() const { return ftree; };
  TString            GetName() const { return fname; };
  int                Fill() { return this->GetTree()->Fill(); };
  bool               Write();
  void               Clear(const int& nFib_x, const int& nFib_y, const int& nFib_z, const int& nFib_xz, const int& nFib_yz);
  static CreateTree* Instance() { return fInstance; };
  static CreateTree* fInstance;
  
  bool Init_data()    const { return this -> INIT_DATA;    };
  bool Deposition()   const { return this -> DEPOSITION;   };
  bool OpPhotons()    const { return this -> OPPHOTONS;    };
  bool Timing()       const { return this -> TIMING;       };
  
  int Event;
  
  float InitialEnergy;
  
  float InitialPositionX;
  float InitialPositionY;
  float InitialPositionZ;
  
  float InitalMomentumDirectionX;
  float InitalMomentumDirectionY;
  float InitalMomentumDirectionZ;
  
  float PrimaryParticleX[1000];
  float PrimaryParticleY[1000];
  float PrimaryParticleZ[1000];
  float PrimaryParticleE[1000];
  
  std::vector<float> depositionE;
  std::vector<float> depositionX;
  std::vector<float> depositionY;
  std::vector<float> depositionZ;
  std::vector<int> depositionCoarseLayer;
  std::vector<int> depositionFineLayer;
  std::vector<int> depositionIX;
  std::vector<int> depositionIY;
  std::vector<float> depositionFiberLocalX;
  std::vector<float> depositionFiberLocalY;
  std::vector<float> depositionFiberLocalZ;
  std::vector<int> depositionType;
  
  float Total_energy_fibers;
  float Total_ion_energy_fibers;
  float Total_nonion_energy_fibers;
  float Total_ion_energy_fibers_att[10];
  float Total_ion_energy_fibers_att_top[10];
  float Total_ion_energy_fibers_att_btm[10];
  std::vector<float> Total_ion_energy_fibers_iX;
  std::vector<float> Total_ion_energy_fibers_iY;
  std::vector<float> Total_ion_energy_fibers_iZ;
  std::vector<float> Total_ion_energy_fibers_iXiZ;
  std::vector<float> Total_ion_energy_fibers_iYiZ;
  
  float Total_energy_absorber;
  float Total_ion_energy_absorber;
  float Total_nonion_energy_absorber;
  
  float Total_energy_preshower;
  float Total_ion_energy_preshower;
  float Total_nonion_energy_preshower;
  
  float Total_energy_postshower;
  float Total_ion_energy_postshower;
  float Total_nonion_energy_postshower;
  
  float Total_energy_sideshower;
  float Total_ion_energy_sideshower;
  float Total_nonion_energy_sideshower;
  
  float Total_energy_world;
  float Total_ion_energy_world;
  float Total_nonion_energy_world;

  int Tot_phot_cer;
  std::vector<int> Tot_phot_cer_iX;
  std::vector<int> Tot_phot_cer_iY;
  std::vector<int> Tot_phot_cer_iZ;
  std::vector<int> Tot_phot_cer_iXiZ;
  std::vector<int> Tot_phot_cer_iYiZ;
  
  float Total_em_energy;
  
  float Radial_stepLength;
  float Longitudinal_stepLength;
  float Radial_ion_energy_absorber[5000];
  float Longitudinal_ion_energy_absorber[1000];
  
  int opPhoton_n;
  std::vector<int> opPhoton_process;
  std::vector<int> opPhoton_trackID;
  std::vector<int> opPhoton_parentTrackID;
  std::vector<int> opPhoton_fiberIX;
  std::vector<int> opPhoton_fiberIY;
  std::vector<int> opPhoton_fiberIZ;
  std::vector<float> opPhoton_energy;
  std::vector<float> opPhoton_prodTime;
  std::vector<float> opPhoton_time;
  //std::vector<float> opPhoton_vertexX;
  //std::vector<float> opPhoton_vertexY;
  //std::vector<float> opPhoton_vertexZ;
  //std::vector<float> opPhoton_pX;
  //std::vector<float> opPhoton_pY;
  //std::vector<float> opPhoton_pZ;
  //std::vector<float> opPhoton_gapProb0;
  //std::vector<float> opPhoton_gapTime0;
  //std::vector<float> opPhoton_gapProb1;
  //std::vector<float> opPhoton_gapTime1;
  //std::vector<float> opPhoton_gapProb2;
  //std::vector<float> opPhoton_gapTime2;
  //std::vector<float> opPhoton_gapProb3;
  //std::vector<float> opPhoton_gapTime3;
  //std::vector<float> opPhoton_gapProb4;
  //std::vector<float> opPhoton_gapTime4;
  
  /*
  int opPhotonFast_n_gap;
  std::vector<int> opPhotonFast_process_gap;
  std::vector<int> opPhotonFast_trackID_gap;
  std::vector<int> opPhotonFast_parentTrackID_gap;
  std::vector<int> opPhotonFast_fiberIX_gap;
  std::vector<int> opPhotonFast_fiberIY_gap;
  std::vector<int> opPhotonFast_fiberIZ_gap;
  std::vector<int> opPhotonFast_side_gap;
  std::vector<float> opPhotonFast_energy_gap;
  std::vector<float> opPhotonFast_prodTime_gap;
  std::vector<float> opPhotonFast_time_gap;
  
  int opPhoton_n_gap;
  std::vector<int> opPhoton_process_gap;
  std::vector<int> opPhoton_trackID_gap;
  std::vector<int> opPhoton_parentTrackID_gap;
  std::vector<int> opPhoton_fiberIX_gap;
  std::vector<int> opPhoton_fiberIY_gap;
  std::vector<int> opPhoton_fiberIZ_gap;
  std::vector<int> opPhoton_side_gap;
  std::vector<float> opPhoton_energy_gap;
  std::vector<float> opPhoton_prodTime_gap;
  std::vector<float> opPhoton_time_gap;
  */
  
  int opPhoton_n_det;
  std::vector<int> opPhoton_process_det;
  std::vector<int> opPhoton_trackID_det;
  std::vector<int> opPhoton_parentTrackID_det;
  std::vector<int> opPhoton_fiberIX_det;
  std::vector<int> opPhoton_fiberIY_det;
  std::vector<int> opPhoton_fiberIZ_det;
  std::vector<int> opPhoton_side_det;
  std::vector<float> opPhoton_energy_det;
  std::vector<float> opPhoton_prodTime_det;
  std::vector<float> opPhoton_time_det;
};

#endif
