#include "event.hh"
#include "G4AnalysisManager.hh"

MyEventAction::MyEventAction(MyRunAction*)
{
    fEdep = 0.;
}

MyEventAction::~MyEventAction()
{}

void MyEventAction::BeginOfEventAction(const G4Event*)
{
    fEdep = 0.;
    wlsParentIDs.clear();
    reflectionRef.clear();
    firstEscapePositions.clear();
    firstEscapeMomentums.clear();
    photonsOutsideWLS.clear();
}

void MyEventAction::EndOfEventAction(const G4Event*)
{
    if(fEdep != 0.)
    {
        //G4cout << "Energy deposition: " << fEdep << G4endl;
    }
    
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleDColumn(0, 0, fEdep);
    man->AddNtupleRow(0);
}