#ifndef __NERIXPHYSICSLIST_H__
#define __NERIXPHYSICSLIST_H__

#include <G4VUserPhysicsList.hh>
#include <globals.hh>

class NerixPhysicsList: public G4VUserPhysicsList
{
public:
	NerixPhysicsList();
	~NerixPhysicsList();

public:
	void SetCuts();

protected:
	void ConstructParticle();
	void ConstructProcess();

	void ConstructGeneral();
	void ConstructEM();
	void ConstructHad();
	void ConstructOp();

	void AddTransportation();

private:
	G4double cutForGamma;
	G4double cutForElectron;
	G4double cutForPositron;
	G4double cutForProton;
	G4double cutForAlpha;
	G4double cutForGenericIon;

	void ConstructMyBosons();
	void ConstructMyLeptons();
	void ConstructMyHadrons();
	void ConstructMyShortLiveds();

private:
	G4int VerboseLevel;
	G4int OpVerbLevel;
};

#endif // __NERIXPHYSICSLIST_H__

