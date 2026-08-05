#ifndef EVENT_HH
#define EVENT_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "run.hh"
#include <set> // Added for std::set usage
#include <map> // Added for std::map usage
#include "G4ThreeVector.hh"

class MyEventAction : public G4UserEventAction
{
public:
    MyEventAction(MyRunAction*);
    ~MyEventAction();
    
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);
    
    void AddEdep(G4double edep) { fEdep += edep; }

    std::set<G4int> recordedPhotonIDs;
    std::set<G4int> wlsParentIDs;
    
    // New set: Tracks WLS photons that have left the 'physWLS' volume but are still alive.
    // This is used by stepping.cc to determine a TRUE final escape.
    std::set<G4int> photonsOutsideWLS; 

    std::map<G4int, G4ThreeVector> firstEscapePositions; 
    std::map<G4int, G4ThreeVector> firstEscapeMomentums;

    std::map<G4int, G4int> reflectionRef; // Map: TrackID -> Reflection Count;
    
private:
    G4double fEdep;
};

#endif