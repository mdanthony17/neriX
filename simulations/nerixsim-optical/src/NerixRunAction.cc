#include <Randomize.hh>

#include <sys/time.h>

#include "NerixAnalysisManager.hh"

#include "NerixRunAction.hh"

NerixRunAction::NerixRunAction(NerixAnalysisManager *pAnalysisManager)
{
	m_pAnalysisManager = pAnalysisManager;
}

NerixRunAction::~NerixRunAction()
{

}

void
NerixRunAction::BeginOfRunAction(const G4Run *pRun)
{
	if(m_pAnalysisManager)
		m_pAnalysisManager->BeginOfRun(pRun);

	struct timeval hTimeValue;
	gettimeofday(&hTimeValue, NULL);

	CLHEP::HepRandom::setTheEngine(new CLHEP::Ranlux64Engine);
	CLHEP::HepRandom::setTheSeed(hTimeValue.tv_usec);
}

void
NerixRunAction::EndOfRunAction(const G4Run *pRun)
{
	if(m_pAnalysisManager)
		m_pAnalysisManager->EndOfRun(pRun);
}
