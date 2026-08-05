#include "run.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "construction.hh"
#include <sstream>

MyRunAction::MyRunAction()
{
    // Get the singleton instance of the analysis manager
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->SetVerboseLevel(1); // Set verbosity for debugging
    man->SetCompressionLevel(0);

    // Ntuple 0: Detected Photons
    man->CreateNtuple("DetPhotons", "Detected Photons Data");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleIColumn("fTrackEvent");
    man->CreateNtupleIColumn("fDet");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->CreateNtupleDColumn("fPx");
    man->CreateNtupleDColumn("fPy");
    man->CreateNtupleDColumn("fPz");
    man->CreateNtupleDColumn("fwlen");
    man->CreateNtupleDColumn("fenergy");
    man->CreateNtupleDColumn("fT");
    man->CreateNtupleIColumn("fReflections");
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
    man->CreateNtupleDColumn("fGlobalTime");
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
    man->CreateNtupleIColumn("fPreVolumeID");
    man->CreateNtupleIColumn("fPostVolumeID");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->CreateNtupleDColumn("fwlen");
    man->CreateNtupleIColumn("fReflections");
    man->CreateNtupleDColumn("fGlobalTime");
    man->FinishNtuple(3);

    // Ntuple 4: Emitted Photons (from WLS)
    man->CreateNtuple("EmmPhotons", "Emitted WLS Photon Data");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleIColumn("fTrackEvent");
    man->CreateNtupleIColumn("fPreVolumeID");
    man->CreateNtupleIColumn("fPostVolumeID");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->CreateNtupleDColumn("fwlen");
    man->CreateNtupleDColumn("fGlobalTime");
    man->FinishNtuple(4);

    // Ntuple 5: Escaped WLS Photons
    man->CreateNtuple("EscapedPhotons", "Escaped WLS Photon Data");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleIColumn("fParentEvent");
    man->CreateNtupleIColumn("fTrackEvent");
    man->CreateNtupleIColumn("fPreVolumeID");
    man->CreateNtupleIColumn("fPostVolumeID");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->CreateNtupleDColumn("fPx"); 
    man->CreateNtupleDColumn("fPy");
    man->CreateNtupleDColumn("fPz");
    man->CreateNtupleDColumn("fwlen");
    man->CreateNtupleIColumn("fReflections");
    man->CreateNtupleDColumn("fGlobalTime");
    man->FinishNtuple(5);

    // Ntuple 6: All Killed WLS Photons
    man->CreateNtuple("WLSPhotons", "Killed WLS Photon Data");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleIColumn("fParentEvent");
    man->CreateNtupleIColumn("fTrackEvent");
    man->CreateNtupleIColumn("fPreVolumeID");
    man->CreateNtupleIColumn("fPostVolumeID");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->CreateNtupleDColumn("fwlen");
    man->CreateNtupleSColumn("fProcessName");
    man->CreateNtupleIColumn("fReflections");
    man->CreateNtupleDColumn("fGlobalTime");
    man->FinishNtuple(6);

    // Ntuple 7: Event Data
    man->CreateNtuple("EventData", "Event Level Data");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleDColumn("fEdep");
    man->FinishNtuple(7);

    // Enable ntuple merging for multi-threaded runs
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

    // 1. Get access to your DetectorConstruction class
    const MyDetectorConstruction* detector = static_cast<const MyDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    // 2. Retrieve values and convert to "Full Size" in mm
    G4double size_mm  = detector->GetWLSSize()/ mm;
    G4double thick_mm = detector->GetWLSThickness()/ mm;
    G4double time_ns  = detector->GetWLSTimeConstant() / ns;
    G4double gap_mm  = detector->GetWLSGap() / mm;

    // 3. Construct the filename
    std::stringstream strFileName;
    strFileName << "/scratch/alice/j/jss55/G4Data/NewMuonGen_RealRef_PolishedWLSPMTBound_ShellPMT/WLS_KR_Ref_Real_Det6_Gap_" 
                << gap_mm << "mm_"
                << "Size_" << size_mm << "mm_"
                << "Thick_" << thick_mm << "mm_" 
                << "Tau_" << time_ns << "ns"
                << ".root";

    // 4. Open the file
    man->OpenFile(strFileName.str());
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    // Write the data to the file and close it
    man->Write();
    man->CloseFile();
}