#include <G4UnitsTable.hh>
#include <G4VVisManager.hh>
#include <G4Circle.hh>
#include <G4Colour.hh>
#include <G4VisAttributes.hh>

#include "NerixLiquidScintillatorHit.hh"

G4Allocator<NerixLiquidScintillatorHit> NerixLiquidScintillatorHitAllocator;

NerixLiquidScintillatorHit::NerixLiquidScintillatorHit() {}

NerixLiquidScintillatorHit::~NerixLiquidScintillatorHit()
{
	if(m_pParticleType) delete m_pParticleType;
	if(m_pDepositingProcess) delete m_pDepositingProcess;
}

NerixLiquidScintillatorHit::NerixLiquidScintillatorHit(const NerixLiquidScintillatorHit &hNerixLiquidScintillatorHit):G4VHit()
{
	m_iLiquidScintillatorId = hNerixLiquidScintillatorHit.m_iLiquidScintillatorId;
	m_iTrackId = hNerixLiquidScintillatorHit.m_iTrackId;
	m_pParticleType = hNerixLiquidScintillatorHit.m_pParticleType;
	m_pDepositingProcess = hNerixLiquidScintillatorHit.m_pDepositingProcess;
	m_dEnergyDeposited = hNerixLiquidScintillatorHit.m_dEnergyDeposited;
	m_dKineticEnergy = hNerixLiquidScintillatorHit.m_dKineticEnergy;
	m_dTime = hNerixLiquidScintillatorHit.m_dTime;
}

const NerixLiquidScintillatorHit &
NerixLiquidScintillatorHit::operator=(const NerixLiquidScintillatorHit &hNerixLiquidScintillatorHit)
{
	m_iLiquidScintillatorId = hNerixLiquidScintillatorHit.m_iLiquidScintillatorId;
	m_iTrackId = hNerixLiquidScintillatorHit.m_iTrackId;
	m_pParticleType = hNerixLiquidScintillatorHit.m_pParticleType;
	m_pDepositingProcess = hNerixLiquidScintillatorHit.m_pDepositingProcess;
	m_dEnergyDeposited = hNerixLiquidScintillatorHit.m_dEnergyDeposited;
	m_dKineticEnergy = hNerixLiquidScintillatorHit.m_dKineticEnergy;
	m_dTime = hNerixLiquidScintillatorHit.m_dTime;
	
	return *this;
}

G4int
NerixLiquidScintillatorHit::operator==(const NerixLiquidScintillatorHit &hNerixLiquidScintillatorHit) const
{
	return ((this == &hNerixLiquidScintillatorHit) ? (1) : (0));
}

void NerixLiquidScintillatorHit::Print()
{
	G4cout << "-------------------- Scintillator hit --------------------" 
		<< "LiquidScintillatorId: " << m_iLiquidScintillatorId
		<< " TrackId: " << m_iTrackId
		<< " Particle: " << *m_pParticleType
		<< " DepositingProcess: " << *m_pDepositingProcess << G4endl
		<< "EnergyDeposited: " << m_dEnergyDeposited/keV << " keV"
		<< " KineticEnergyLeft: " << m_dKineticEnergy/keV << " keV"
		<< " Time: " << m_dTime/s << " s" << G4endl;
}

