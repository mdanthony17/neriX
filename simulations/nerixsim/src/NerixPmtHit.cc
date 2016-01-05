#include <G4UnitsTable.hh>
#include <G4VVisManager.hh>
#include <G4Circle.hh>
#include <G4Colour.hh>
#include <G4VisAttributes.hh>

#include "NerixPmtHit.hh"

G4Allocator<NerixPmtHit> NerixPmtHitAllocator;

NerixPmtHit::NerixPmtHit() {}

NerixPmtHit::~NerixPmtHit() {}

NerixPmtHit::NerixPmtHit(const NerixPmtHit &hNerixPmtHit):G4VHit()
{
	m_hPosition = hNerixPmtHit.m_hPosition;
	m_dAngle = hNerixPmtHit.m_dAngle;
	m_dTime = hNerixPmtHit.m_dTime;
	m_iPmtNb = hNerixPmtHit.m_iPmtNb;
	m_iQuadrantNb = hNerixPmtHit.m_iQuadrantNb;
}

const NerixPmtHit &
NerixPmtHit::operator=(const NerixPmtHit &hNerixPmtHit)
{
	m_hPosition = hNerixPmtHit.m_hPosition;
	m_dAngle = hNerixPmtHit.m_dAngle;
	m_dTime = hNerixPmtHit.m_dTime;
	m_iPmtNb = hNerixPmtHit.m_iPmtNb;
	m_iQuadrantNb = hNerixPmtHit.m_iQuadrantNb;
	
	return *this;
}

G4int
NerixPmtHit::operator==(const NerixPmtHit &hNerixPmtHit) const
{
	return ((this == &hNerixPmtHit) ? (1) : (0));
}

void NerixPmtHit::Draw()
{
//    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
//    
//    if(pVVisManager)
//    {
//        G4Circle hCircle(m_hPosition);
//        G4Colour hColour(1.000, 0.973, 0.184);
//        G4VisAttributes hVisAttributes(hColour);
//        
//        hCircle.SetScreenSize(0.1);
//        hCircle.SetFillStyle(G4Circle::filled);
//        hCircle.SetVisAttributes(hVisAttributes);
//        pVVisManager->Draw(hCircle);
//    }
}

void NerixPmtHit::Print()
{
	G4cout << "Pmt hit ---> " 
		<< "Pmt#" << m_iPmtNb
		<< " Quadrant#" << m_iQuadrantNb
		<< " Position: " << m_hPosition.x()/mm << " " << m_hPosition.y()/mm << " " << m_hPosition.z()/mm << " mm"
		<< " Angle: " << m_dAngle/deg << " deg"
		<< " Time: " << m_dTime/s << " s" << G4endl;
}

