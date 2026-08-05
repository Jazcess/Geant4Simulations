#include "stepping.hh"
#include "event.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4Gamma.hh"
#include "G4OpticalPhoton.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4ProcessManager.hh"

G4OpBoundaryProcess* GetBoundaryProcess(const G4ParticleDefinition* particleDef) {
    G4ProcessManager* pManager = particleDef->GetProcessManager();
    G4int pVectorLength = pManager->GetProcessListLength();
    G4ProcessVector* pVector = pManager->GetProcessList();
    for (G4int i = 0; i < pVectorLength; i++) {
        if ((*pVector)[i]->GetProcessName() == "OpBoundary") {
            return (G4OpBoundaryProcess*)(*pVector)[i];
        }
    }
    return nullptr;
}

inline G4double CalculateWavelength(const G4ThreeVector& momentum)
{
    return (1.239841939 * eV / momentum.mag()) * 1E+03; 
}

MySteppingAction::MySteppingAction(MyEventAction *eventAction) : fEventAction(eventAction)
{}

MySteppingAction::~MySteppingAction()
{}

void MySteppingAction::UserSteppingAction(const G4Step *step)
{
    const G4Track* track = step->GetTrack();
    const G4ParticleDefinition* particleDef = track->GetDefinition();

    if (particleDef == G4MuonPlus::Definition() || particleDef == G4MuonMinus::Definition())
    {
        fEventAction->AddEdep(step->GetTotalEnergyDeposit());
        return; 
    }

    if (particleDef == G4Gamma::GammaDefinition() || particleDef == G4OpticalPhoton::OpticalPhotonDefinition())
    {
        G4int trackID = track->GetTrackID(); 
        G4int parentID = track->GetParentID();
        G4StepPoint* preStepPoint = step->GetPreStepPoint();
        G4StepPoint* postStepPoint = step->GetPostStepPoint();
        
        G4VPhysicalVolume* preVolume = preStepPoint->GetPhysicalVolume();
        G4int preVolumeID = preVolume->GetCopyNo();
        
        G4AnalysisManager *man = G4AnalysisManager::Instance();
        G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

        if (track->GetCurrentStepNumber() == 1 && track->GetCreatorProcess() && track->GetCreatorProcess()->GetProcessName() == "Cerenkov") {
            G4ThreeVector origPos = preStepPoint->GetPosition();
            man->FillNtupleIColumn(1, 0, evt);      
            man->FillNtupleIColumn(1, 1, trackID);      
            man->FillNtupleIColumn(1, 2, preVolumeID);  
            man->FillNtupleDColumn(1, 3, origPos.x());  
            man->FillNtupleDColumn(1, 4, origPos.y());  
            man->FillNtupleDColumn(1, 5, origPos.z());  
            man->FillNtupleDColumn(1, 6, CalculateWavelength(preStepPoint->GetMomentum()));
            man->FillNtupleDColumn(1, 7, preStepPoint->GetGlobalTime()); 
            man->AddNtupleRow(1);                       
        }

        if (postStepPoint->GetStepStatus() == fGeomBoundary) {
            static G4OpBoundaryProcess* boundaryProc = GetBoundaryProcess(particleDef);
            
            if (boundaryProc) {
                G4OpBoundaryProcessStatus status = boundaryProc->GetStatus();
                if (status == TotalInternalReflection || status == FresnelReflection || 
                    status == SpikeReflection || status == LambertianReflection) {
                    fEventAction->reflectionRef[trackID]++;
                }
            }

            if (fEventAction->wlsParentIDs.count(parentID)) {
                G4VPhysicalVolume* postVolume = postStepPoint->GetPhysicalVolume();
                if (postVolume) {
                    if (preVolume->GetName() == "physWLS" && postVolume->GetName() == "physWater") {
                        if (fEventAction->firstEscapePositions.find(trackID) == fEventAction->firstEscapePositions.end()) {
                            fEventAction->firstEscapePositions[trackID] = postStepPoint->GetPosition(); 
                            fEventAction->firstEscapeMomentums[trackID] = postStepPoint->GetMomentumDirection();
                        }
                        fEventAction->photonsOutsideWLS.insert(trackID);
                    } 
                    else if (preVolume->GetName() != "physWLS" && postVolume->GetName() == "physWLS") {
                        fEventAction->photonsOutsideWLS.erase(trackID);
                    }
                }
            }
        }

        if (track->GetTrackStatus() == fStopAndKill) {
            const G4VProcess* process = postStepPoint->GetProcessDefinedStep();
            G4String processName = process ? process->GetProcessName() : "Unknown";
            G4VPhysicalVolume* postVolume = postStepPoint->GetPhysicalVolume();
            G4int postVolumeID = postVolume ? postVolume->GetCopyNo() : -1;
            
            G4ThreeVector pos = postStepPoint->GetPosition();
            G4double wlen = CalculateWavelength(preStepPoint->GetMomentum());
            G4int reflCount = fEventAction->reflectionRef[trackID];
            G4double globalTime = postStepPoint->GetGlobalTime();

            if (processName == "OpAbsorption" || processName == "OpWLS") {
                man->FillNtupleIColumn(3, 0, evt);      
                man->FillNtupleIColumn(3, 1, trackID);    
                man->FillNtupleIColumn(3, 2, preVolumeID);
                man->FillNtupleIColumn(3, 3, postVolumeID);
                man->FillNtupleDColumn(3, 4, pos.x());
                man->FillNtupleDColumn(3, 5, pos.y());
                man->FillNtupleDColumn(3, 6, pos.z());
                man->FillNtupleDColumn(3, 7, wlen);
                man->FillNtupleIColumn(3, 8, reflCount);
                man->FillNtupleDColumn(3, 9, globalTime); 
                man->AddNtupleRow(3);

                const std::vector<const G4Track*>* secondaries = step->GetSecondaryInCurrentStep();
                for (const auto* secondaryTrack : *secondaries) {
                    if (secondaryTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) {
                        fEventAction->wlsParentIDs.insert(trackID);
                        G4ThreeVector secPos = secondaryTrack->GetPosition();
                        man->FillNtupleIColumn(4, 0, evt);          
                        man->FillNtupleIColumn(4, 1, secondaryTrack->GetTrackID());      
                        man->FillNtupleIColumn(4, 2, preVolumeID);
                        man->FillNtupleIColumn(4, 3, postVolumeID);
                        man->FillNtupleDColumn(4, 4, secPos.x());
                        man->FillNtupleDColumn(4, 5, secPos.y());
                        man->FillNtupleDColumn(4, 6, secPos.z());
                        man->FillNtupleDColumn(4, 7, CalculateWavelength(secondaryTrack->GetMomentum()));
                        man->FillNtupleDColumn(4, 8, secondaryTrack->GetGlobalTime()); 
                        man->AddNtupleRow(4);
                    }
                }
            }

            if (fEventAction->wlsParentIDs.count(parentID)) {
                if (fEventAction->photonsOutsideWLS.count(trackID)) {
                    if (fEventAction->firstEscapePositions.count(trackID)) {
                        G4ThreeVector escapePos = fEventAction->firstEscapePositions[trackID];
                        G4ThreeVector escapeMom = fEventAction->firstEscapeMomentums[trackID];
                        
                        man->FillNtupleIColumn(5, 0, evt);      
                        man->FillNtupleIColumn(5, 1, parentID);     
                        man->FillNtupleIColumn(5, 2, trackID);      
                        man->FillNtupleIColumn(5, 3, preVolumeID); 
                        man->FillNtupleIColumn(5, 4, postVolumeID); 
                        man->FillNtupleDColumn(5, 5, escapePos.x());
                        man->FillNtupleDColumn(5, 6, escapePos.y());
                        man->FillNtupleDColumn(5, 7, escapePos.z());
                        man->FillNtupleDColumn(5, 8, escapeMom.x());
                        man->FillNtupleDColumn(5, 9, escapeMom.y());
                        man->FillNtupleDColumn(5, 10, escapeMom.z());
                        man->FillNtupleDColumn(5, 11, wlen);
                        man->FillNtupleIColumn(5, 12, reflCount); 
                        man->FillNtupleDColumn(5, 13, globalTime);
                        man->AddNtupleRow(5);

                        fEventAction->photonsOutsideWLS.erase(trackID);
                        fEventAction->firstEscapePositions.erase(trackID); 
                        fEventAction->firstEscapeMomentums.erase(trackID); 
                    }
                }
                
                if (process) {
                    man->FillNtupleIColumn(6, 0, evt);          
                    man->FillNtupleIColumn(6, 1, parentID);     
                    man->FillNtupleIColumn(6, 2, trackID);      
                    man->FillNtupleIColumn(6, 3, preVolumeID);
                    man->FillNtupleIColumn(6, 4, postVolumeID);
                    man->FillNtupleDColumn(6, 5, pos.x());      
                    man->FillNtupleDColumn(6, 6, pos.y());
                    man->FillNtupleDColumn(6, 7, pos.z());
                    man->FillNtupleDColumn(6, 8, wlen);
                    man->FillNtupleSColumn(6, 9, processName);
                    man->FillNtupleIColumn(6, 10, reflCount);
                    man->FillNtupleDColumn(6, 11, globalTime); 
                    man->AddNtupleRow(6);
                }
            }
        }
    }
}