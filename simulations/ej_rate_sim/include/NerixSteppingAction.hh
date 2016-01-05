#ifndef __NERIXSTEPPINGACTION_H__
#define __NERIXSTEPPINGACTION_H__

#include <globals.hh>
#include <G4UserSteppingAction.hh>

class NerixAnalysisManager;

class NerixSteppingAction: public G4UserSteppingAction
{
public:
	NerixSteppingAction(NerixAnalysisManager *pAnalysisManager=0);
	~NerixSteppingAction();
  
	virtual void UserSteppingAction(const G4Step *pStep);

private:
	NerixAnalysisManager *m_pAnalysisManager;
};

#endif // __NERIXSTEPPINGACTION_H__

