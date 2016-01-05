#ifndef __NERIXLIQUIDSCINTILLATORSENSITIVEDETECTOR_H__
#define __NERIXLIQUIDSCINTILLATORSENSITIVEDETECTOR_H__

#include <G4VSensitiveDetector.hh>

#include "NerixLiquidScintillatorHit.hh"

class G4Step;
class G4HCofThisEvent;

class NerixLiquidScintillatorSensitiveDetector: public G4VSensitiveDetector
{
public:
	NerixLiquidScintillatorSensitiveDetector(G4String hName, G4String hCollectionName);
	~NerixLiquidScintillatorSensitiveDetector();

	void Initialize(G4HCofThisEvent *pHitsCollectionOfThisEvent);
	G4bool ProcessHits(G4Step *pStep, G4TouchableHistory *pHistory);
	void EndOfEvent(G4HCofThisEvent *pHitsCollectionOfThisEvent);

private:
	G4int m_iHitsCollectionID;
	NerixLiquidScintillatorHitsCollection* m_pScintillatorHitsCollection;
};

#endif // __NERIXLIQUIDSCINTILLATORSENSITIVEDETECTOR_H__

