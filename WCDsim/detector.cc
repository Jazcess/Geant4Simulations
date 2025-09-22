#include "detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name), lastEventID(-1)
{
    quEff = new G4PhysicsFreeVector();

    std::ifstream datafile;
    datafile.open("eff.dat");

    while(1)
    {
        G4double wlen, queff;

        datafile >> wlen >> queff;

        if(datafile.eof())
        break;

        //G4cout << wlen << " " << queff << G4endl;

        quEff->InsertValues(wlen, queff);
    }

    datafile.close();

}

MySensitiveDetector::~MySensitiveDetector()
{}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{

    G4Track *track = aStep->GetTrack();
    G4int trackID = track->GetTrackID();

    track->SetTrackStatus(fStopAndKill);

    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

    G4ThreeVector posPhoton = preStepPoint->GetPosition();
    G4ThreeVector momPhoton = preStepPoint->GetMomentum();

    G4double wlen = (1.239841939*eV/momPhoton.mag())*1E+03;
    G4double time = preStepPoint->GetGlobalTime();
    G4double energy = track->GetKineticEnergy()*10e5; //in eV

    //G4cout << "Photon position: " << posPhoton << G4endl;

    const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();

    G4int copyNo = touchable->GetCopyNumber();

    //G4cout << "Copy Number: " << copyNo << G4endl;

    G4VPhysicalVolume *physVol = touchable->GetVolume();
    G4ThreeVector posDetector = physVol->GetTranslation();


    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();



    G4AnalysisManager *man = G4AnalysisManager::Instance();

    //For quantum efficiency
    if(G4UniformRand() < quEff->Value(wlen))
    {

    man->FillNtupleIColumn(0, 0, evt);
    man->FillNtupleIColumn(0, 1, trackID);
    man->FillNtupleIColumn(0, 2, copyNo);   // Detector ID (copy number)
    man->FillNtupleDColumn(0, 3, posPhoton[0]);
    man->FillNtupleDColumn(0, 4, posPhoton[1]);
    man->FillNtupleDColumn(0, 5, posPhoton[2]);
    man->FillNtupleDColumn(0, 6, wlen);
    man->FillNtupleDColumn(0, 7, energy);
    man->FillNtupleDColumn(0, 8, time);
    man->AddNtupleRow(0);
    }



	return true;
}
