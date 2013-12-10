#ifndef SteppingAction_H
#define SteppingAction_H 1

#include "G4UserSteppingAction.hh"
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
#include "DetectorConstruction.hh"
#include "MyMaterials.hh"

#include <iostream>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"



class SteppingAction : public G4UserSteppingAction
{
public:

  SteppingAction(const DetectorConstruction* detectorConstruction);
  ~SteppingAction();
  virtual void UserSteppingAction(const G4Step*);
  
  
private:
  const DetectorConstruction* fDetectorConstruction;
};

#endif
