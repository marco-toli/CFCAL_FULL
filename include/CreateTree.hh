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
  Bool_t	       ENERGY_FIBER;
  Bool_t	       INIT_DATA; 
  Bool_t	       POS_FIBER;
  Bool_t	       OPTICAL;

  public:

  CreateTree(TString name, Bool_t energy_fiber, Bool_t init_data, Bool_t pos_fiber, Bool_t optical);
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
  
};
