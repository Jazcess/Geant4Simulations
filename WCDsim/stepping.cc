#include "stepping.hh"
#include "event.hh" // Assuming event.hh contains MyEventAction
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpBoundaryProcess.hh"

MySteppingAction::MySteppingAction(MyEventAction *eventAction)
{
    fEventAction = eventAction;
}

MySteppingAction::~MySteppingAction()
{}

void MySteppingAction::UserSteppingAction(const G4Step *step)
{
    // Get the particle type from the track
    const G4Track* track = step->GetTrack();
    const G4ParticleDefinition* particleDef = track->GetDefinition();
    G4int trackID = track->GetTrackID(); // Changed from photonID for clarity
    G4StepPoint* postStepPoint = step->GetPostStepPoint();
    const G4VProcess* process = postStepPoint->GetProcessDefinedStep();
    G4StepPoint* preStepPoint = step->GetPreStepPoint();
    G4String processName = process ? process->GetProcessName() : "Unknown";
                
    // Get the physical volume of the current and post step
    G4VPhysicalVolume* preVolume = preStepPoint->GetPhysicalVolume();
    G4VPhysicalVolume* postVolume = postStepPoint->GetPhysicalVolume();

    G4int preVolumeID  = preVolume->GetCopyNo();
    G4int postVolumeID = postVolume ? postVolume->GetCopyNo() : -1;

    G4int parentID = track->GetParentID();

    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition *muon = particleTable->FindParticle("mu+");
    G4ParticleDefinition *gamma = particleTable->FindParticle("gamma");



    // Check if the particle is a muon
    if (particleDef == G4MuonPlus::Definition() || particleDef == G4MuonMinus::Definition())
    {
        G4double edep = step->GetTotalEnergyDeposit();
        fEventAction->AddEdep(edep);
    }

    // Cherenkov photon generation tracking
    if (track->GetCreatorProcess() && track->GetCreatorProcess()->GetProcessName() == "Cerenkov") {

        // Only record if this photon has not been recorded yet
        if (fEventAction->recordedPhotonIDs.find(trackID) == fEventAction->recordedPhotonIDs.end()) {

            fEventAction->recordedPhotonIDs.insert(trackID);

            G4ThreeVector origPos = preStepPoint->GetPosition();
            // Calculate the photon wavelength
            G4double wlen = (1.239841939 * eV / preStepPoint->GetMomentum().mag()) * 1E+03;

            // Fill the ntuple with Cherenkov photon data
            G4AnalysisManager *man = G4AnalysisManager::Instance();
            G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
            man->FillNtupleIColumn(1, 0, evt);           // Event ID
            man->FillNtupleIColumn(1, 1, trackID);       // Unique Photon ID
            man->FillNtupleIColumn(1, 2, preVolumeID);   // Chamber ID (1 or 2)
            man->FillNtupleDColumn(1, 3, origPos.x());   // Photon X position
            man->FillNtupleDColumn(1, 4, origPos.y());   // Photon Y position
            man->FillNtupleDColumn(1, 5, origPos.z());   // Photon Z position
            man->FillNtupleDColumn(1, 6, wlen);          // Photon wavelength
            man->AddNtupleRow(1);                        // Add row to ntuple
        }
    }


    // Check if the particle is a photon
    if (particleDef == gamma || particleDef->GetParticleName() == "opticalphoton"){

        // Check if the photon is absorbed or involved in WLS
        if (processName == "OpAbsorption" || processName == "OpWLS") {

            // Record absorption if the track is killed
            if (track->GetTrackStatus() == fStopAndKill){

                G4ThreeVector position = postStepPoint->GetPosition();
                G4double wlen = (1.239841939 * eV / preStepPoint->GetMomentum().mag()) * 1E+03;

                G4AnalysisManager *man = G4AnalysisManager::Instance();
                G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
                man->FillNtupleIColumn(3, 0, evt);        // Event ID
                man->FillNtupleIColumn(3, 1, trackID);    // Unique Photon ID
                man->FillNtupleIColumn(3, 2, preVolumeID);
                man->FillNtupleDColumn(3, 3, position.x());
                man->FillNtupleDColumn(3, 4, position.y());
                man->FillNtupleDColumn(3, 5, position.z());
                man->FillNtupleDColumn(3, 6, wlen);
                man->AddNtupleRow(3);
            }

            // Check if a new photon was created (re-emitted via WLS)
            const std::vector<const G4Track*>* secondaries = step->GetSecondaryInCurrentStep();
            for (const auto* secondaryTrack : *secondaries) {
                if (secondaryTrack->GetDefinition()->GetParticleName() == "opticalphoton") {
                    // The original photon is the parent
                    fEventAction->wlsParentIDs.insert(trackID);

                    // Record emission data
                    G4ThreeVector position = secondaryTrack->GetPosition();
                    G4double wlen = (1.239841939 * eV / secondaryTrack->GetMomentum().mag()) * 1E+03;

                    G4AnalysisManager *man = G4AnalysisManager::Instance();
                    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
                    man->FillNtupleIColumn(4, 0, evt);           // Event ID
                    man->FillNtupleIColumn(4, 1, trackID);       // Unique Photon ID of the parent
                    man->FillNtupleIColumn(4, 2, preVolumeID);
                    man->FillNtupleDColumn(4, 3, position.x());
                    man->FillNtupleDColumn(4, 4, position.y());
                    man->FillNtupleDColumn(4, 5, position.z());
                    man->FillNtupleDColumn(4, 6, wlen);
                    man->AddNtupleRow(4);
                }
            }
        }

        // Is this photon a WLS secondary?
        if (fEventAction->wlsParentIDs.find(parentID) != fEventAction->wlsParentIDs.end()) {
            // This photon originated from a WLS emission

            // Photon stopped
            if (track->GetTrackStatus() == fStopAndKill) {

                if (process) {
                    G4ThreeVector pos = postStepPoint->GetPosition();
                    G4double wlen = (1.239841939 * eV / preStepPoint->GetMomentum().mag()) * 1E+03;

                    G4AnalysisManager *man = G4AnalysisManager::Instance();
                    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
   
                    // Fill ntuple for WLS photons (Ntuple 6)
                    man->FillNtupleIColumn(6, 0, evt);           // Event ID
                    man->FillNtupleIColumn(6, 1, parentID);      // Parent photon ID
                    man->FillNtupleIColumn(6, 2, trackID);       // Photon ID
                    man->FillNtupleIColumn(6, 3, postVolumeID);  // Volume ID where it died
                    man->FillNtupleDColumn(6, 4, pos.x());
                    man->FillNtupleDColumn(6, 5, pos.y());
                    man->FillNtupleDColumn(6, 6, pos.z());
                    man->FillNtupleDColumn(6, 7, wlen);
                    man->FillNtupleSColumn(6, 8, processName);   // Store process that killed photon
                    man->AddNtupleRow(6);
                }

                // Check if it died outside of WLS volume (escaped)
                if (postVolume && postVolume->GetName() != "physWLS") {

                    if (preVolume != postVolume) {
                        G4ThreeVector position = postStepPoint->GetPosition();
                        G4double wlen = (1.239841939 * eV / preStepPoint->GetMomentum().mag()) * 1E+03;

                        G4AnalysisManager *man = G4AnalysisManager::Instance();
                        G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
                        // Ntuple for escaped WLS photons (Ntuple 5)
                        man->FillNtupleIColumn(5, 0, evt);           // Event ID
                        man->FillNtupleIColumn(5, 1, parentID);      // Unique Parent ID
                        man->FillNtupleIColumn(5, 2, trackID);       // Unique Photon ID
                        man->FillNtupleIColumn(5, 3, postVolumeID);  // ID of volume photon was killed in
                        man->FillNtupleDColumn(5, 4, position.x());
                        man->FillNtupleDColumn(5, 5, position.y());
                        man->FillNtupleDColumn(5, 6, position.z());
                        man->FillNtupleDColumn(5, 7, wlen);
                        man->AddNtupleRow(5);
                    }
                }
            }
        }
    }
}

