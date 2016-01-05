#include <G4HCofThisEvent.hh>
#include <G4Step.hh>
#include <G4VProcess.hh>
#include <G4ThreeVector.hh>
#include <G4SDManager.hh>
#include <G4ios.hh>

#include <map>

using std::map;

#include "NerixLiquidScintillatorSensitiveDetector.hh"

NerixLiquidScintillatorSensitiveDetector::NerixLiquidScintillatorSensitiveDetector(G4String hName, G4String hCollectionName): G4VSensitiveDetector(hName)
{
	m_iHitsCollectionID = -1;
	collectionName.insert(hCollectionName);
}

NerixLiquidScintillatorSensitiveDetector::~NerixLiquidScintillatorSensitiveDetector()
{
}

void NerixLiquidScintillatorSensitiveDetector::Initialize(G4HCofThisEvent* pHitsCollectionOfThisEvent)
{
	m_pScintillatorHitsCollection = new NerixLiquidScintillatorHitsCollection(SensitiveDetectorName, collectionName[0]);

	if(m_iHitsCollectionID < 0)
		m_iHitsCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
	
	pHitsCollectionOfThisEvent->AddHitsCollection(m_iHitsCollectionID, m_pScintillatorHitsCollection);
}

G4bool NerixLiquidScintillatorSensitiveDetector::ProcessHits(G4Step* pStep, G4TouchableHistory *pHistory)
{
	G4double dEnergyDeposited = pStep->GetTotalEnergyDeposit();
	G4Track *pTrack = pStep->GetTrack();

	NerixLiquidScintillatorHit* pHit = new NerixLiquidScintillatorHit();

	G4int iLiquidScintillatorId = pTrack->GetTouchable()->GetVolume(1)->GetCopyNo();

	pHit->SetLiquidScintillatorId(iLiquidScintillatorId);
	pHit->SetTrackId(pTrack->GetTrackID());
	pHit->SetParticleType(pTrack->GetDefinition()->GetParticleName());
	pHit->SetDepositingProcess(pStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName());
	pHit->SetEnergyDeposited(dEnergyDeposited);
	pHit->SetKineticEnergy(pTrack->GetKineticEnergy());
	pHit->SetTime(pTrack->GetGlobalTime());

	m_pScintillatorHitsCollection->insert(pHit);

	return true;
}

void NerixLiquidScintillatorSensitiveDetector::EndOfEvent(G4HCofThisEvent *pHitsCollectionOfThisEvent)
{
}

