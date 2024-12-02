#include "detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{}

MySensitiveDetector::~MySensitiveDetector()
{}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
	G4Track *track = aStep->GetTrack(); //find the track that hit the sensitive detector

	track->SetTrackStatus(fStopAndKill); //stop and kill the track when it has hit the sensitive detector

	G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

	G4ThreeVector posPhoton = preStepPoint->GetPosition();
	G4ThreeVector momPhoton = preStepPoint->GetMomentum();

	G4double wlen = (1.239841939*eV/momPhoton.mag())*1E+03;
	G4double time = preStepPoint->GetGlobalTime();

	//G4cout << "Photon position: " << posPhoton << G4endl;

	const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();

	G4int copyNo = touchable->GetCopyNumber(); //detector/pixel ID

	//G4cout << "Copy Number: " << copyNo << G4endl;

	G4VPhysicalVolume *physVol = touchable->GetVolume();
	G4ThreeVector posDetector = physVol->GetTranslation();

	G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();


	G4AnalysisManager *man = G4AnalysisManager::Instance();

	//write to file/table

	man->FillNtupleIColumn(0, 0, evt);
	man->FillNtupleIColumn(0, 1, copyNo);
	man->FillNtupleDColumn(0, 2, posPhoton[0]);
	man->FillNtupleDColumn(0, 3, posPhoton[1]);
	man->FillNtupleDColumn(0, 4, posPhoton[2]);
	man->FillNtupleDColumn(0, 5, wlen);
	man->FillNtupleDColumn(0, 6, time);
	man->AddNtupleRow(0);



	return true;
}
