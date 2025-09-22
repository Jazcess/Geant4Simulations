#include "run.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include <sstream>

MyRunAction::MyRunAction()
{
    // Get the singleton instance of the analysis manager
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->SetVerboseLevel(1); // Set verbosity for debugging

    // Ntuple 0: Detected Photons
    man->CreateNtuple("DetPhotons", "Detected Photons Data");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleIColumn("fTrackEvent");
    man->CreateNtupleIColumn("fDet");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->CreateNtupleDColumn("fwlen");
    man->CreateNtupleDColumn("fenergy");
    man->CreateNtupleDColumn("fT");
    man->FinishNtuple(0);

    // Ntuple 1: Cherenkov Photons
    man->CreateNtuple("Cherenkov", "Cherenkov Photon Generation Data");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleIColumn("fTrackEvent");
    man->CreateNtupleIColumn("fChamberID");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->CreateNtupleDColumn("fwlen");
    man->CreateNtupleDColumn("fEdep");
    man->FinishNtuple(1);

    // Ntuple 2: Muons
    man->CreateNtuple("Muons", "Primary Muon Data");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->CreateNtupleDColumn("fXMom");
    man->CreateNtupleDColumn("fYMom");
    man->CreateNtupleDColumn("fZMom");
    man->CreateNtupleDColumn("ftheta");
    man->CreateNtupleDColumn("fphi");
    man->FinishNtuple(2);

    // Ntuple 3: Absorbed Photons
    man->CreateNtuple("AbsPhotons", "Absorbed Photon Data");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleIColumn("fTrackEvent");
    man->CreateNtupleIColumn("fVolumeID");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->CreateNtupleDColumn("fwlen");
    man->CreateNtupleIColumn("fReflections"); // <-- UPDATED: Added column for reflection count
    man->FinishNtuple(3);

    // Ntuple 4: Emitted Photons (from WLS)
    man->CreateNtuple("EmmPhotons", "Emitted WLS Photon Data");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleIColumn("fTrackEvent"); // This is the parent track ID
    man->CreateNtupleIColumn("fVolumeID");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->CreateNtupleDColumn("fwlen");
    man->FinishNtuple(4);

    // Ntuple 5: Escaped WLS Photons
    man->CreateNtuple("EscapedPhotons", "Escaped WLS Photon Data");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleIColumn("fParentEvent");
    man->CreateNtupleIColumn("fTrackEvent");
    man->CreateNtupleIColumn("fVolumeID");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->CreateNtupleDColumn("fwlen");
    man->CreateNtupleIColumn("fReflections"); // <-- UPDATED: Added column for reflection count
    man->FinishNtuple(5);

    // Ntuple 6: All Killed WLS Photons
    man->CreateNtuple("WLSPhotons", "Killed WLS Photon Data");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleIColumn("fParentEvent");
    man->CreateNtupleIColumn("fTrackEvent");
    man->CreateNtupleIColumn("fVolumeID");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->CreateNtupleDColumn("fwlen");
    man->CreateNtupleSColumn("fProcessName");
    man->CreateNtupleIColumn("fReflections"); // <-- UPDATED: Added column for reflection count
    man->FinishNtuple(6);

    // Enable ntuple merging for multi-threaded runs
    // This check is important to avoid issues in sequential mode
    if (G4RunManager::GetRunManager()->GetRunManagerType() == G4RunManager::masterRM)
    {
        man->SetNtupleMerging(true);
    }
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    // Create a unique filename for each run
    G4int runID = run->GetRunID();
    std::stringstream strRunID;
    strRunID << runID;

    // Open an output file
    man->OpenFile("data/output" + strRunID.str() + ".root");
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    // Write the data to the file and close it
    man->Write();
    man->CloseFile();
}
