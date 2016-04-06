#include <G4HCofThisEvent.hh>
#include <G4Step.hh>
#include <G4VProcess.hh>
#include <G4ThreeVector.hh>
#include <G4SDManager.hh>
#include <G4ios.hh>

#include <map>

using std::map;

#include "NerixGeDetectorSensitiveDetector.hh"

NerixGeDetectorSensitiveDetector::NerixGeDetectorSensitiveDetector(G4String hName, G4String hCollectionName): G4VSensitiveDetector(hName)
{
	m_iHitsCollectionID = -1;
	collectionName.insert(hCollectionName);
}

NerixGeDetectorSensitiveDetector::~NerixGeDetectorSensitiveDetector()
{
}

void NerixGeDetectorSensitiveDetector::Initialize(G4HCofThisEvent* pHitsCollectionOfThisEvent)
{
	m_pGeHitsCollection = new NerixGeDetectorHitsCollection(SensitiveDetectorName, collectionName[0]);

	if(m_iHitsCollectionID < 0)
		m_iHitsCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
	
	pHitsCollectionOfThisEvent->AddHitsCollection(m_iHitsCollectionID, m_pGeHitsCollection);
}

G4bool NerixGeDetectorSensitiveDetector::ProcessHits(G4Step* pStep, G4TouchableHistory *pHistory)
{
	G4double dEnergyDeposited = pStep->GetTotalEnergyDeposit();
	G4Track *pTrack = pStep->GetTrack();

	NerixGeDetectorHit* pHit = new NerixGeDetectorHit();

	G4int iGeDetectorId = pTrack->GetTouchable()->GetVolume(1)->GetCopyNo();

	pHit->SetGeDetectorId(iGeDetectorId);
	pHit->SetTrackId(pTrack->GetTrackID());
	pHit->SetParticleType(pTrack->GetDefinition()->GetParticleName());
	pHit->SetDepositingProcess(pStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName());
	pHit->SetEnergyDeposited(dEnergyDeposited);
	pHit->SetKineticEnergy(pTrack->GetKineticEnergy());
	pHit->SetTime(pTrack->GetGlobalTime());

	m_pGeHitsCollection->insert(pHit);

	return true;
}

void NerixGeDetectorSensitiveDetector::EndOfEvent(G4HCofThisEvent *pHitsCollectionOfThisEvent)
{
}

