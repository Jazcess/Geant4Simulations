#ifndef EVENT_HH
#define EVENT_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "run.hh"

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

    // Map to store reflection counts for each photon track ID
    std::map<G4int, G4int> photonReflectionCounts;
    
private:
    G4double fEdep;
};

#endif
