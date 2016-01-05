#include <G4UnitsTable.hh>
#include <G4VVisManager.hh>
#include <G4Circle.hh>
#include <G4Colour.hh>
#include <G4VisAttributes.hh>

#include "NerixLXeHit.hh"

G4Allocator<NerixLXeHit> NerixLXeHitAllocator;

NerixLXeHit::NerixLXeHit()
{
	m_pParticleType = 0;
	m_pParentType = 0;
	m_pCreatorProcess = 0;
	m_pDepositingProcess = 0;
}

NerixLXeHit::~NerixLXeHit()
{
	if(m_pParticleType) delete m_pParticleType;
	if(m_pParentType) delete m_pParentType;
	if(m_pCreatorProcess) delete m_pCreatorProcess;
	if(m_pDepositingProcess) delete m_pDepositingProcess;
}

NerixLXeHit::NerixLXeHit(const NerixLXeHit &hNerixLXeHit):G4VHit()
{
	m_iTrackId = hNerixLXeHit.m_iTrackId;
	m_iParentId = hNerixLXeHit.m_iParentId;
	m_pParticleType = hNerixLXeHit.m_pParticleType;
	m_pParentType = hNerixLXeHit.m_pParentType ;
	m_pCreatorProcess = hNerixLXeHit.m_pCreatorProcess ;
	m_pDepositingProcess = hNerixLXeHit.m_pDepositingProcess ;
	m_hPosition = hNerixLXeHit.m_hPosition;
	m_dEnergyDeposited = hNerixLXeHit.m_dEnergyDeposited;
	m_dKineticEnergy = hNerixLXeHit.m_dKineticEnergy ;
	m_dTime = hNerixLXeHit.m_dTime;
}

const NerixLXeHit &
NerixLXeHit::operator=(const NerixLXeHit &hNerixLXeHit)
{
	m_iTrackId = hNerixLXeHit.m_iTrackId;
	m_iParentId = hNerixLXeHit.m_iParentId;
	m_pParticleType = hNerixLXeHit.m_pParticleType;
	m_pParentType = hNerixLXeHit.m_pParentType ;
	m_pCreatorProcess = hNerixLXeHit.m_pCreatorProcess ;
	m_pDepositingProcess = hNerixLXeHit.m_pDepositingProcess ;
	m_hPosition = hNerixLXeHit.m_hPosition;
	m_dEnergyDeposited = hNerixLXeHit.m_dEnergyDeposited;
	m_dKineticEnergy = hNerixLXeHit.m_dKineticEnergy ;
	m_dTime = hNerixLXeHit.m_dTime;
	
	return *this;
}

G4int
NerixLXeHit::operator==(const NerixLXeHit &hNerixLXeHit) const
{
	return ((this == &hNerixLXeHit) ? (1) : (0));
}

void NerixLXeHit::Draw()
{
	G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
	
	if(pVVisManager)
	{
		G4Circle hCircle(m_hPosition);
		G4Colour hColour(1.000, 0.973, 0.184);
		G4VisAttributes hVisAttributes(hColour);
		
		hCircle.SetScreenSize(0.1);
		hCircle.SetFillStyle(G4Circle::filled);
		hCircle.SetVisAttributes(hVisAttributes);
		pVVisManager->Draw(hCircle);
	}
}

void NerixLXeHit::Print()
{
	G4cout << "-------------------- LXe hit --------------------" 
		<< "Id: " << m_iTrackId
		<< " Particle: " << *m_pParticleType
		<< " ParentId: " << m_iParentId
		<< " ParentType: " << *m_pParentType << G4endl
		<< "CreatorProcess: " << *m_pCreatorProcess
		<< " DepositingProcess: " << *m_pDepositingProcess << G4endl
		<< "Position: " << m_hPosition.x()/mm
		<< " " << m_hPosition.y()/mm
		<< " " << m_hPosition.z()/mm
		<< " mm" << G4endl
		<< "EnergyDeposited: " << m_dEnergyDeposited/keV << " keV"
		<< " KineticEnergyLeft: " << m_dKineticEnergy/keV << " keV"
		<< " Time: " << m_dTime/s << " s" << G4endl;
}

