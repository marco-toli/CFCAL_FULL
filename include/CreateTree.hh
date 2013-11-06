// Martin Goettlich @ DESY
//
#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include <vector>

using namespace std;

class CreateTree
{
private:
  
  TTree*              ftree;
  TString             fname;

public:
  Bool_t	      ENERGY_FIBER;
  Bool_t	      INIT_DATA; 
  Bool_t	      POS_FIBER;
  Bool_t	      OPTICAL;
  Bool_t              TIMING;
  
public:
  
  CreateTree(TString name, Bool_t energy_fiber, Bool_t init_data, Bool_t pos_fiber, Bool_t optical, Bool_t timing);
  ~CreateTree();

  TTree*              GetTree() const { return ftree; };
  TString             GetName() const { return fname;};
  Int_t               Fill() { return this->GetTree()->Fill(); };
  Bool_t              Write();
  void                Clear();
  static CreateTree*  Instance() { return fInstance; };
  static CreateTree*  fInstance;
  
  Bool_t	       Energy_fiber() const { return this -> ENERGY_FIBER; };
  Bool_t	       Init_data()    const { return this -> INIT_DATA;    };
  Bool_t	       Pos_fiber()    const { return this -> POS_FIBER;    };
  Bool_t	       Optical()      const { return this -> OPTICAL;	   };
  
  Int_t               Event;
  
  Float_t InitialPositionX;
  Float_t InitialPositionY;
  Float_t InitialPositionZ;
  
  Float_t InitalMomentumDirectionX;
  Float_t InitalMomentumDirectionY;
  Float_t InitalMomentumDirectionZ;

  vector<Float_t> Energy_deposited;
  vector<Float_t> depositionX;
  vector<Float_t> depositionY;
  vector<Float_t> depositionZ;
  
  Float_t Total_energy[250][300];
  Float_t Total_nonion_energy[250][300];
  Int_t   Num_phot_cer[250][300];
  
  Float_t Total_energy_fibers;
  Float_t Total_nonion_energy_fibers;  
  Float_t Tot_phot_cer;  
  Float_t Total_energy_absorber;
  Float_t Total_energy_world;
  
  Int_t opPhoton_n;
  vector<Int_t> opPhoton_process;
  vector<Int_t> opPhoton_trackID;
  vector<Int_t> opPhoton_parentTrackID;
  vector<Int_t> opPhoton_fiberIX;
  vector<Int_t> opPhoton_fiberIY;
  vector<Float_t> opPhoton_energy;
  vector<Float_t> opPhoton_waveLength;
  vector<Float_t> opPhoton_time;
  vector<Float_t> opPhoton_vertexX;
  vector<Float_t> opPhoton_vertexY;
  vector<Float_t> opPhoton_vertexZ;
  vector<Float_t> opPhoton_pX;
  vector<Float_t> opPhoton_pY;
  vector<Float_t> opPhoton_pZ;
  
  Int_t opPhoton_n_det;
  vector<Int_t> opPhoton_process_det;
  vector<Int_t> opPhoton_trackID_det;
  vector<Int_t> opPhoton_parentTrackID_det;
  vector<Int_t> opPhoton_fiberIX_det;
  vector<Int_t> opPhoton_fiberIY_det;
  vector<Float_t> opPhoton_energy_det;
  vector<Float_t> opPhoton_waveLength_det;
  vector<Float_t> opPhoton_time_det;
};
