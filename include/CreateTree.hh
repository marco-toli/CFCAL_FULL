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
  void               Clear();
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
  std::vector<int> depositionType;
  
  float Total_energy_fibers;
  float Total_ion_energy_fibers;
  float Total_nonion_energy_fibers;
  float Total_ion_energy_fibers_iX[1000];
  float Total_ion_energy_fibers_iY[1000];
  float Total_ion_energy_fibers_iZ[1000];
  
  float Total_energy_absorber;
  float Total_ion_energy_absorber;
  float Total_nonion_energy_absorber;
    
  float Total_energy_world;
  float Total_ion_energy_world;
  float Total_nonion_energy_world;

  float Tot_phot_cer;
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
  std::vector<float> opPhoton_waveLength;
  std::vector<float> opPhoton_time;
  std::vector<float> opPhoton_vertexX;
  std::vector<float> opPhoton_vertexY;
  std::vector<float> opPhoton_vertexZ;
  std::vector<float> opPhoton_pX;
  std::vector<float> opPhoton_pY;
  std::vector<float> opPhoton_pZ;
  
  int opPhoton_n_det;
  std::vector<int> opPhoton_process_det;
  std::vector<int> opPhoton_trackID_det;
  std::vector<int> opPhoton_parentTrackID_det;
  std::vector<int> opPhoton_fiberIX_det;
  std::vector<int> opPhoton_fiberIY_det;
  std::vector<int> opPhoton_fiberIZ_det;
  std::vector<float> opPhoton_energy_det;
  std::vector<float> opPhoton_waveLength_det;
  std::vector<float> opPhoton_time_det;
};

#endif
