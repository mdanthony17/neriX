#include <G4ios.hh>
#include <G4ParticleDefinition.hh>
#include <G4ParticleTypes.hh>
#include <G4Track.hh>
#include <G4Event.hh>
#include <G4VProcess.hh>
#include <G4StackManager.hh>

#include "NerixAnalysisManager.hh"

#include "NerixSteppingAction.hh"

NerixSteppingAction::NerixSteppingAction(NerixAnalysisManager *pAnalysisManager)
{
	m_pAnalysisManager = pAnalysisManager;
}

NerixSteppingAction::~NerixSteppingAction()
{
}

void
NerixSteppingAction::UserSteppingAction(const G4Step *pStep)
{
	if(m_pAnalysisManager)
		m_pAnalysisManager->Step(pStep);
}

