#include <G4UnitsTable.hh>
#include <G4VVisManager.hh>
#include <G4Circle.hh>
#include <G4Colour.hh>
#include <G4VisAttributes.hh>

#include "NerixGeDetectorHit.hh"

G4Allocator<NerixGeDetectorHit> NerixGeDetectorHitAllocator;

NerixGeDetectorHit::NerixGeDetectorHit() {}

NerixGeDetectorHit::~NerixGeDetectorHit()
{
	if(m_pParticleType) delete m_pParticleType;
	if(m_pDepositingProcess) delete m_pDepositingProcess;
}

NerixGeDetectorHit::NerixGeDetectorHit(const NerixGeDetectorHit &hNerixGeDetectorHit):G4VHit()
{
	m_iGeDetectorId = hNerixGeDetectorHit.m_iGeDetectorId;
	m_iTrackId = hNerixGeDetectorHit.m_iTrackId;
	m_pParticleType = hNerixGeDetectorHit.m_pParticleType;
	m_pDepositingProcess = hNerixGeDetectorHit.m_pDepositingProcess;
	m_dEnergyDeposited = hNerixGeDetectorHit.m_dEnergyDeposited;
	m_dKineticEnergy = hNerixGeDetectorHit.m_dKineticEnergy;
	m_dTime = hNerixGeDetectorHit.m_dTime;
}

const NerixGeDetectorHit &
NerixGeDetectorHit::operator=(const NerixGeDetectorHit &hNerixGeDetectorHit)
{
	m_iGeDetectorId = hNerixGeDetectorHit.m_iGeDetectorId;
	m_iTrackId = hNerixGeDetectorHit.m_iTrackId;
	m_pParticleType = hNerixGeDetectorHit.m_pParticleType;
	m_pDepositingProcess = hNerixGeDetectorHit.m_pDepositingProcess;
	m_dEnergyDeposited = hNerixGeDetectorHit.m_dEnergyDeposited;
	m_dKineticEnergy = hNerixGeDetectorHit.m_dKineticEnergy;
	m_dTime = hNerixGeDetectorHit.m_dTime;
	
	return *this;
}

G4int
NerixGeDetectorHit::operator==(const NerixGeDetectorHit &hNerixGeDetectorHit) const
{
	return ((this == &hNerixGeDetectorHit) ? (1) : (0));
}

void NerixGeDetectorHit::Print()
{
	G4cout << "-------------------- NaI hit --------------------" 
		<< "GeDetectorId: " << m_iGeDetectorId
		<< " TrackId: " << m_iTrackId
		<< " Particle: " << *m_pParticleType
		<< " DepositingProcess: " << *m_pDepositingProcess << G4endl
		<< "EnergyDeposited: " << m_dEnergyDeposited/keV << " keV"
		<< " KineticEnergyLeft: " << m_dKineticEnergy/keV << " keV"
		<< " Time: " << m_dTime/s << " s" << G4endl;
}

