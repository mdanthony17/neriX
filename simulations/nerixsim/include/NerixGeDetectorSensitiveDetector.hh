#ifndef __NERIXGEDETECTORSENSITIVEDETECTOR_H__
#define __NERIXGEDETECTORSENSITIVEDETECTOR_H__

#include <G4VSensitiveDetector.hh>

#include "NerixGeDetectorHit.hh"

class G4Step;
class G4HCofThisEvent;

class NerixGeDetectorSensitiveDetector: public G4VSensitiveDetector
{
public:
	NerixGeDetectorSensitiveDetector(G4String hName, G4String hCollectionName);
	~NerixGeDetectorSensitiveDetector();

	void Initialize(G4HCofThisEvent *pHitsCollectionOfThisEvent);
	G4bool ProcessHits(G4Step *pStep, G4TouchableHistory *pHistory);
	void EndOfEvent(G4HCofThisEvent *pHitsCollectionOfThisEvent);

private:
	G4int m_iHitsCollectionID;
	NerixGeDetectorHitsCollection* m_pGeHitsCollection;
};

#endif // __NERIXGEDETECTORSENSITIVEDETECTOR_H__

