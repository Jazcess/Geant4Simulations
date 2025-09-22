#include "event.hh"

MyEventAction::MyEventAction(MyRunAction*)
{
    fEdep = 0.;
}

MyEventAction::~MyEventAction()
{}

void MyEventAction::BeginOfEventAction(const G4Event*)
{
    fEdep = 0.;
    recordedPhotonIDs.clear(); 
    wlsParentIDs.clear();
    photonReflectionCounts.clear();
}

void MyEventAction::EndOfEventAction(const G4Event*)
{

    if(fEdep != 0.)
    {
        //G4cout << "Energy deposition: " << fEdep << G4endl;
    };
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleDColumn(1, 7, fEdep);
    man->AddNtupleRow(1);

}
