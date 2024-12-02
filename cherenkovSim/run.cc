#include "run.hh"

MyRunAction::MyRunAction()
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();

    //Creating data tables, to save as .csv files
    man->CreateNtuple("Photons", "Photons");
	man->CreateNtupleIColumn("fEvent");
	man->CreateNtupleIColumn("fDetID");
	man->CreateNtupleDColumn("fX");
	man->CreateNtupleDColumn("fY");
	man->CreateNtupleDColumn("fZ");
	man->CreateNtupleDColumn("fwlen");
	man->CreateNtupleDColumn("fT");
	man->FinishNtuple(0);

}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();

	G4int runID = run->GetRunID();

	std::stringstream strRunID;
	strRunID << runID;

	//man->OpenFile("data/output"+strRunID.str()+".csv"); //save as .csv file
	man->OpenFile("data/output"+strRunID.str()+".root"); //save as .root file


}

void MyRunAction::EndOfRunAction(const G4Run*)
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();

	man->Write();
	man->CloseFile();
}
