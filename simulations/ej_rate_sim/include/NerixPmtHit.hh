#ifndef __NERIXPMTHIT_H__
#define __NERIXPMTHIT_H__

#include <G4VHit.hh>
#include <G4THitsCollection.hh>
#include <G4Allocator.hh>
#include <G4ThreeVector.hh>

class NerixPmtHit: public G4VHit
{
public:
	NerixPmtHit();
	~NerixPmtHit();
	NerixPmtHit(const NerixPmtHit &);
	const NerixPmtHit & operator=(const NerixPmtHit &);
	G4int operator==(const NerixPmtHit &) const;

	inline void* operator new(size_t);
	inline void  operator delete(void*);

	void Draw();
	void Print();

public:
	void SetPosition(G4ThreeVector hPosition) { m_hPosition = hPosition; }
	void SetAngle(G4double dAngle) { m_dAngle = dAngle; }
	void SetTime(G4double dTime) { m_dTime = dTime; }
	void SetPmtNb(G4int iPmtNb) { m_iPmtNb = iPmtNb; }
	void SetQuadrantNb(G4int iQuadrantNb) { m_iQuadrantNb = iQuadrantNb; }

	G4ThreeVector GetPosition() { return m_hPosition; }
	G4double GetAngle() { return m_dAngle; }
	G4double GetTime() { return m_dTime; }
	G4int GetPmtNb() { return m_iPmtNb; }
	G4int GetQuadrantNb() { return m_iQuadrantNb; }

private:
	G4ThreeVector m_hPosition;
	G4double m_dAngle;
	G4double m_dTime;
	G4int m_iPmtNb;
	G4int m_iQuadrantNb;
};

typedef G4THitsCollection<NerixPmtHit> NerixPmtHitsCollection;

extern G4Allocator<NerixPmtHit> NerixPmtHitAllocator;

inline void*
NerixPmtHit::operator new(size_t)
{
	return((void *) NerixPmtHitAllocator.MallocSingle());
}

inline void
NerixPmtHit::operator delete(void *pNerixPmtHit)
{
	NerixPmtHitAllocator.FreeSingle((NerixPmtHit*) pNerixPmtHit);
}

#endif // __NERIXPMTHIT_H__

