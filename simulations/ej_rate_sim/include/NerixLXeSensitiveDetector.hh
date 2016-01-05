#ifndef __NERIXLXESENSITIVEDETECTOR_H__
#define __NERIXLXESENSITIVEDETECTOR_H__

#include <G4VSensitiveDetector.hh>

#include "NerixLXeHit.hh"

#include <map>

class G4Step;
class G4HCofThisEvent;

class NerixLXeSensitiveDetector: public G4VSensitiveDetector
{
public:
	NerixLXeSensitiveDetector(G4String hName);
	~NerixLXeSensitiveDetector();

	void Initialize(G4HCofThisEvent *pHitsCollectionOfThisEvent);
	G4bool ProcessHits(G4Step *pStep, G4TouchableHistory *pHistory);
	void EndOfEvent(G4HCofThisEvent *pHitsCollectionOfThisEvent);

private:
	NerixLXeHitsCollection* m_pLXeHitsCollection;

	std::map<int,G4String> m_hParticleTypes;
};

#endif // __NERIXLXESENSITIVEDETECTOR_H__

