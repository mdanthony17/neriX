#ifndef __NERIXRUNACTION_H__
#define __NERIXRUNACTION_H__

#include <G4UserRunAction.hh>

class G4Run;

class NerixAnalysisManager;

class NerixRunAction: public G4UserRunAction
{
public:
	NerixRunAction(NerixAnalysisManager *pAnalysisManager=0);
	~NerixRunAction();

public:
	void BeginOfRunAction(const G4Run *pRun);
	void EndOfRunAction(const G4Run *pRun);

private:
	NerixAnalysisManager *m_pAnalysisManager;
};

#endif // __NERIXRUNACTION_H__

