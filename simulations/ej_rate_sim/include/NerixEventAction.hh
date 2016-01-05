#ifndef __NERIXEVENTACTION_H__
#define __NERIXEVENTACTION_H__

#include <G4UserEventAction.hh>

#include "NerixAnalysisManager.hh"

class G4Event;

class NerixEventAction : public G4UserEventAction
{
public:
	NerixEventAction(NerixAnalysisManager *pAnalysisManager = 0);
	~NerixEventAction();

public:
	void BeginOfEventAction(const G4Event *pEvent);
	void EndOfEventAction(const G4Event *pEvent);

private:
	NerixAnalysisManager *m_pAnalysisManager;
};

#endif // __NERIXEVENTACTION_H__

