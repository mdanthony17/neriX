#include <G4HCofThisEvent.hh>
#include <G4Step.hh>
#include <G4VProcess.hh>
#include <G4ThreeVector.hh>
#include <G4SDManager.hh>
#include <G4ios.hh>

#include <map>

using namespace std;

#include "NerixPmtSensitiveDetector.hh"

NerixPmtSensitiveDetector::NerixPmtSensitiveDetector(G4String hName): G4VSensitiveDetector(hName)
{
	collectionName.insert("PmtHitsCollection");
}

NerixPmtSensitiveDetector::~NerixPmtSensitiveDetector()
{
}

void NerixPmtSensitiveDetector::Initialize(G4HCofThisEvent* pHitsCollectionOfThisEvent)
{
	m_pPmtHitsCollection = new NerixPmtHitsCollection(SensitiveDetectorName, collectionName[0]);

	static G4int iHitsCollectionID = -1;

	if(iHitsCollectionID < 0)
		iHitsCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
	
	pHitsCollectionOfThisEvent->AddHitsCollection(iHitsCollectionID, m_pPmtHitsCollection); 
}

G4bool NerixPmtSensitiveDetector::ProcessHits(G4Step* pStep, G4TouchableHistory *pHistory)
{
	G4Track *pTrack = pStep->GetTrack();

	if(pTrack->GetDefinition()->GetParticleName() == "opticalphoton")
	{
		G4int iPmtNb = pTrack->GetTouchable()->GetVolume(2)->GetCopyNo();

		G4ThreeVector hMomentumDirection = pTrack->GetMomentumDirection();

		/*
		G4double dX = (iPmtNb == 1 || iPmtNb == 3)?((iPmtNb == 1)?(1.):(-1.)):(0.);
		G4double dY = (iPmtNb == 2 || iPmtNb == 4)?((iPmtNb == 2)?(1.):(-1.)):(0.);
		G4double dZ = (iPmtNb == 0 || iPmtNb == 5)?((iPmtNb == 0)?(1.):(-1.)):(0.);
		*/
		G4double dX = 0.;
		G4double dY = 0.;
		G4double dZ = (iPmtNb < 4)?(1.):(-1.);

		G4ThreeVector hPmtDirection(dX, dY, dZ);

		G4double dAngle = hMomentumDirection.angle(hPmtDirection);

		NerixPmtHit* pHit = new NerixPmtHit();

		G4ThreeVector hPosition = pStep->GetPreStepPoint()->GetPosition();
//        G4cout << "PhysicalVolume: " << pStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() << G4endl;
//        G4cout << "PmtWindowPosition: " << pTrack->GetTouchable()->GetVolume(2)->GetTranslation() << G4endl;

		if(iPmtNb != 4)
		{
			G4ThreeVector hPmtPosition = pTrack->GetTouchable()->GetVolume(2)->GetTranslation();
			G4ThreeVector hDelta = hPosition-hPmtPosition;

			G4int iQuadrantNb = (hDelta.x() > 0)?((hDelta.y() > 0)?(1):(3)):((hDelta.y() > 0)?(0):(2));
//            G4cout << "QuadrantNb: " << iQuadrantNb << G4endl;
			pHit->SetQuadrantNb(iQuadrantNb);
		}
		else
			pHit->SetQuadrantNb(0);

		pHit->SetPosition(hPosition);

		pHit->SetAngle(dAngle);
		pHit->SetTime(pTrack->GetGlobalTime());
		pHit->SetPmtNb(iPmtNb);

		m_pPmtHitsCollection->insert(pHit);

//        pHit->Print();
//        pHit->Draw();

		return true;
	}
	else
	{
		return false;
	}
}

void NerixPmtSensitiveDetector::EndOfEvent(G4HCofThisEvent *pHitsCollectionOfThisEvent)
{
//  if (verboseLevel>0) { 
//     G4int NbHits = trackerCollection->entries();
//     G4cout << "\n-------->Hits Collection: in this event they are " << NbHits 
//            << " hits in the tracker chambers: " << G4endl;
//     for (G4int i=0;i<NbHits;i++) (*trackerCollection)[i]->Print();
//    } 
}

