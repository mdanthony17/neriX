#include <globals.hh>
#include <G4RunManagerKernel.hh>
#include <G4Event.hh>
#include <Randomize.hh>

#include <sys/time.h>
#include <sstream>
#include <iostream>

#include "NerixParticleSource.hh"

#include "NerixPrimaryGeneratorAction.hh"

NerixPrimaryGeneratorAction::NerixPrimaryGeneratorAction()
{
	m_pParticleSource = new NerixParticleSource();

	m_hParticleTypeOfPrimary = "neutron";
	m_dEnergyOfPrimary = 2.5*MeV;
	//m_hPositionOfPrimary = G4ThreeVector(0., -1*m, -15*mm);
	m_hPositionOfPrimary = G4ThreeVector(0., 400*mm, -15*mm);
    m_hDirectionOfPrimary = G4ThreeVector(0., 1., 0.);
	
	std::cout << "Generating neutron source\n";

	m_hEngineStatus = std::vector<unsigned long>();

	m_pHepRandomEngine = new CLHEP::RanluxEngine();
	CLHEP::HepRandom::setTheEngine(m_pHepRandomEngine);

	struct timeval hTimeValue;
	gettimeofday(&hTimeValue, NULL);

	CLHEP::HepRandom::setTheSeed(hTimeValue.tv_usec);
}

NerixPrimaryGeneratorAction::~NerixPrimaryGeneratorAction()
{
	delete m_pParticleSource;
}

void
NerixPrimaryGeneratorAction::GeneratePrimaries(G4Event *pEvent)
{
	if(!m_hInitialEngineStatus.empty())
	{
		m_pHepRandomEngine->getState(m_hInitialEngineStatus);
		m_hInitialEngineStatus.clear();
	}

	m_hEngineStatus = m_pHepRandomEngine->put();

//    G4cout << "Generate Primaries" << G4endl;
//    CLHEP::HepRandom::showEngineStatus();

	G4StackManager *pStackManager = (G4RunManagerKernel::GetRunManagerKernel())->GetStackManager();

//    G4cout << "PrimaryGeneratorAction: track status: "
//        << pStackManager->GetNUrgentTrack() << " urgent, "
//        << pStackManager->GetNWaitingTrack() << " waiting, "
//        << pStackManager->GetNPostponedTrack() << " postponed"
//        << G4endl;

	if(!pStackManager->GetNPostponedTrack())
	{
		m_pParticleSource->GeneratePrimaryVertex(pEvent);
	}
	else
	{
		pStackManager->TransferStackedTracks(fPostpone, fUrgent);
		G4VTrajectory* pTrajectory;
		G4Track *pTrack = pStackManager->PopNextTrack(&pTrajectory);

		m_pParticleSource->GeneratePrimaryVertexFromTrack(pTrack, pEvent);

		delete pTrack;
	}
	G4PrimaryVertex *pVertex = pEvent->GetPrimaryVertex();
	G4PrimaryParticle *pPrimaryParticle = pVertex->GetPrimary();

	m_hParticleTypeOfPrimary = pPrimaryParticle->GetG4code()->GetParticleName();

	G4double dP = pPrimaryParticle->GetMomentum().mag();
	G4double dMass = pPrimaryParticle->GetMass();

	m_dEnergyOfPrimary = std::sqrt(dP*dP + dMass*dMass) - dMass;
	m_hPositionOfPrimary = pVertex->GetPosition();
	m_hDirectionOfPrimary = pPrimaryParticle->GetMomentum().unit();
}

