// Martin Goettlich @ DESY
//
 
#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "G4Event.hh"

#include "MyMaterials.hh"
#include "CreateTree.hh"
#include "DetectorConstruction.hh"

#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"


class G4Event;

class EventAction : public G4UserEventAction
{
public:
  EventAction(const DetectorConstruction* detectorConstruction);
  ~EventAction();
  
  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);
  
private:
  const DetectorConstruction* fDetectorConstruction;
};

#endif
