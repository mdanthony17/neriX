#ifndef __NERIXGEDETECTORHIT_H__
#define __NERIXGEDETECTORHIT_H__

#include <G4VHit.hh>
#include <G4THitsCollection.hh>
#include <G4Allocator.hh>
#include <G4ThreeVector.hh>

class NerixGeDetectorHit: public G4VHit
{
public:
	NerixGeDetectorHit();
	~NerixGeDetectorHit();
	NerixGeDetectorHit(const NerixGeDetectorHit &);
	const NerixGeDetectorHit & operator=(const NerixGeDetectorHit &);
	G4int operator==(const NerixGeDetectorHit &) const;

	inline void* operator new(size_t);
	inline void  operator delete(void*);

	void Print();

public:
	void SetGeDetectorId(G4int iGeDetectorId) { m_iGeDetectorId = iGeDetectorId; }
	void SetTrackId(G4int iTrackId) { m_iTrackId = iTrackId; };
	void SetParticleType(const G4String &hParticleType) { m_pParticleType = new G4String(hParticleType); }
	void SetDepositingProcess(const G4String &hProcessName) { m_pDepositingProcess = new G4String(hProcessName); }
	void SetEnergyDeposited(G4double dEnergyDeposited) { m_dEnergyDeposited = dEnergyDeposited; };
	void SetKineticEnergy(G4double dKineticEnergy) { m_dKineticEnergy = dKineticEnergy; };
	void SetTime(G4double dTime) { m_dTime = dTime; };

	G4int GetGeDetectorId() { return m_iGeDetectorId; };
	G4int GetTrackId() { return m_iTrackId; };
	const G4String &GetParticleType() { return *m_pParticleType; }
	const G4String &GetDepositingProcess() { return *m_pDepositingProcess; }
	G4double GetEnergyDeposited() { return m_dEnergyDeposited; };      
	G4double GetKineticEnergy() { return m_dKineticEnergy; };      
	G4double GetTime() { return m_dTime; };      

private:
	G4int m_iGeDetectorId;
	G4int m_iTrackId;
	G4String *m_pParticleType;
	G4String *m_pDepositingProcess;
	G4double m_dEnergyDeposited;
	G4double m_dKineticEnergy;
	G4double m_dTime;
};

typedef G4THitsCollection<NerixGeDetectorHit> NerixGeDetectorHitsCollection;

extern G4Allocator<NerixGeDetectorHit> NerixGeDetectorHitAllocator;

inline void*
NerixGeDetectorHit::operator new(size_t)
{
	return((void *) NerixGeDetectorHitAllocator.MallocSingle());
}

inline void
NerixGeDetectorHit::operator delete(void *pNerixGeDetectorHit)
{
	NerixGeDetectorHitAllocator.FreeSingle((NerixGeDetectorHit*) pNerixGeDetectorHit);
}

#endif // __NERIXGEDETECTORHIT_H__

