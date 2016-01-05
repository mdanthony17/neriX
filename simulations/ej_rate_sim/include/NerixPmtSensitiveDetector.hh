#ifndef __NERIXPMTSENSITIVEDETECTOR_H__
#define __NERIXPMTSENSITIVEDETECTOR_H__

#include <G4VSensitiveDetector.hh>

#include "NerixPmtHit.hh"

class G4Step;
class G4HCofThisEvent;

class NerixPmtSensitiveDetector: public G4VSensitiveDetector
{
public:
	NerixPmtSensitiveDetector(G4String hName);
	~NerixPmtSensitiveDetector();

	void Initialize(G4HCofThisEvent *pHitsCollectionOfThisEvent);
	G4bool ProcessHits(G4Step *pStep, G4TouchableHistory *pHistory);
	void EndOfEvent(G4HCofThisEvent *pHitsCollectionOfThisEvent);

private:
	NerixPmtHitsCollection* m_pPmtHitsCollection;
};

#endif // __NERIXPMTSENSITIVEDETECTOR_H__

