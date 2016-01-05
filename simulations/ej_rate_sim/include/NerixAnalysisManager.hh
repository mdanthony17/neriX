#ifndef __NERIXANALYSISMANAGER_H__
#define __NERIXANALYSISMANAGER_H__

#include <globals.hh>

#include <TParameter.h>

#include <vector>

class G4Run;
class G4Event;
class G4Step;

class TFile;
class TTree;

class NerixEventData;
class NerixPrimaryGeneratorAction;

class NerixAnalysisManager
{
public:
	NerixAnalysisManager(NerixPrimaryGeneratorAction *pPrimaryGeneratorAction);
	virtual ~NerixAnalysisManager();

public:
	virtual void BeginOfRun(const G4Run *pRun); 
	virtual void EndOfRun(const G4Run *pRun); 
	virtual void BeginOfEvent(const G4Event *pEvent); 
	virtual void EndOfEvent(const G4Event *pEvent); 
	virtual void Step(const G4Step *pStep);	

	void SetDataFilename(const G4String &hFilename) { m_hDataFilename = hFilename; }
	void SetNbEventsToSimulate(G4int iNbEventsToSimulate) { m_iNbEventsToSimulate = iNbEventsToSimulate; }

private:
	G4bool FilterEvent(NerixEventData *pEventData);

private:
	G4int m_iLXeHitsCollectionID;
	G4int m_iLiquidScintillatorHitsCollectionID;
	G4int m_iGeDetectorHitsCollectionID;
	G4int m_iPmtHitsCollectionID;

	G4String m_hDataFilename;
	G4int m_iNbEventsToSimulate;

	TFile *m_pTreeFile;
	TTree *m_pTree;
	TParameter<int> *m_pNbEventsToSimulateParameter;

	NerixPrimaryGeneratorAction *m_pPrimaryGeneratorAction;

	G4double m_dEnergyInLiquidScintillator;
	std::vector<G4String> m_hNeutronVolumes;

	G4double m_dEnergyInGeDetector;
	std::vector<G4String> m_hGammaVolumes;

	NerixEventData *m_pEventData;
};

#endif // __NERIXANALYSISMANAGER_H__

