#include "detector.hh"
#include "event.hh"
#include "G4OpticalPhoton.hh" // Required for the definition check

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

        quEff->InsertValues(wlen, queff);
    }

    datafile.close();
}

MySensitiveDetector::~MySensitiveDetector()
{}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    G4Track *track = aStep->GetTrack();

    // --- CRITICAL FIX: Ignore anything that isn't an optical photon ---
    // This allows Muons/Electrons to pass through without being recorded or killed.
    if(track->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()) 
    {
        return false; 
    }

    // Now we know it's a photon, we can stop and count it.
    track->SetTrackStatus(fStopAndKill);

    G4int trackID = track->GetTrackID();

    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    
    G4ThreeVector posPhoton = preStepPoint->GetPosition();
    G4ThreeVector momPhoton = preStepPoint->GetMomentum();
    G4ThreeVector momDir = preStepPoint->GetMomentumDirection();

    // Calculate wavelength (using Planck's constant hc ~ 1240 eV*nm)
    // Note: your manual conversion is fine, just ensure units match.
    G4double wlen = (1.239841939*eV/momPhoton.mag())*1E+03;
    
    G4double time = preStepPoint->GetGlobalTime();
    G4double energy = track->GetKineticEnergy(); // Removed 10e5 scaling (unless you strictly need it scaled differently)

    const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();
    G4int copyNo = touchable->GetCopyNumber();
    
    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    // --- GET REFLECTION COUNT ---
    MyEventAction *evtAction = (MyEventAction*)G4RunManager::GetRunManager()->GetUserEventAction();
    G4int reflectionCount = 0;
    if (evtAction && evtAction->reflectionRef.count(trackID)) {
        reflectionCount = evtAction->reflectionRef[trackID];
    }

    G4AnalysisManager *man = G4AnalysisManager::Instance();

    // Check Quantum Efficiency (Uncomment this if you want realistic detection probabilities!)
    if(G4UniformRand() < quEff->Value(wlen)) 
    {
        man->FillNtupleIColumn(0, 0, evt);
        man->FillNtupleIColumn(0, 1, trackID);
        man->FillNtupleIColumn(0, 2, copyNo);
        man->FillNtupleDColumn(0, 3, posPhoton[0]);
        man->FillNtupleDColumn(0, 4, posPhoton[1]);
        man->FillNtupleDColumn(0, 5, posPhoton[2]);
        man->FillNtupleDColumn(0, 6, momDir.x());
        man->FillNtupleDColumn(0, 7, momDir.y()); 
        man->FillNtupleDColumn(0, 8, momDir.z()); 
        man->FillNtupleDColumn(0, 9, wlen);
        man->FillNtupleDColumn(0, 10, energy);
        man->FillNtupleDColumn(0, 11, time);
        man->FillNtupleIColumn(0, 12, reflectionCount);
        man->AddNtupleRow(0);
    }

    return true;
}