#ifndef __NERIXPRIMARYGENERATORACTION_H__
#define __NERIXPRIMARYGENERATORACTION_H__

//#include </Users/Matt/Geant4/CLHEP/2.0.3.1/CLHEP/Random/src/DRand48Engine.cc>

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4ThreeVector.hh>
#include <Randomize.hh>
#include <globals.hh>

#include <vector>

class NerixParticleSource;

class G4Event;

class NerixPrimaryGeneratorAction: public G4VUserPrimaryGeneratorAction
{
public:
	NerixPrimaryGeneratorAction();
	~NerixPrimaryGeneratorAction();

public:
	void SetInitialEngineStatus(const std::vector<unsigned long> &hInitialEngineStatus) { m_hInitialEngineStatus = hInitialEngineStatus; }
	const std::vector<unsigned long> &GetEngineStatus() { return m_hEngineStatus; }
	const G4String &GetParticleTypeOfPrimary() { return m_hParticleTypeOfPrimary; }
	G4double GetEnergyOfPrimary() { return m_dEnergyOfPrimary; }
	G4ThreeVector GetPositionOfPrimary() { return m_hPositionOfPrimary; }
	G4ThreeVector GetDirectionOfPrimary() { return m_hDirectionOfPrimary; }

	void GeneratePrimaries(G4Event *pEvent);

  private:
	CLHEP::RanluxEngine *m_pHepRandomEngine;
	std::vector<unsigned long> m_hInitialEngineStatus;
	std::vector<unsigned long> m_hEngineStatus;

	G4String m_hParticleTypeOfPrimary;
	G4double m_dEnergyOfPrimary;
	G4ThreeVector m_hPositionOfPrimary;
	G4ThreeVector m_hDirectionOfPrimary;

	NerixParticleSource *m_pParticleSource;
};

#endif // __NERIXPRIMARYGENERATORACTION_H__

