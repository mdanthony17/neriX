#ifndef __NERIXLXEHIT_H__
#define __NERIXLXEHIT_H__

#include <G4VHit.hh>
#include <G4THitsCollection.hh>
#include <G4Allocator.hh>
#include <G4ThreeVector.hh>

class NerixLXeHit: public G4VHit
{
public:
	NerixLXeHit();
	~NerixLXeHit();
	NerixLXeHit(const NerixLXeHit &);
	const NerixLXeHit & operator=(const NerixLXeHit &);
	G4int operator==(const NerixLXeHit &) const;

	inline void* operator new(size_t);
	inline void  operator delete(void*);

	void Draw();
	void Print();

public:
	void SetTrackId(G4int iTrackId) { m_iTrackId = iTrackId; };
	void SetParentId(G4int iParentId) { m_iParentId = iParentId; };
	void SetParticleType(const G4String &hParticleType) { m_pParticleType = new G4String(hParticleType); }
	void SetParentType(const G4String &hParentType) { m_pParentType = new G4String(hParentType); }
	void SetCreatorProcess(const G4String &hProcessName) { m_pCreatorProcess = new G4String(hProcessName); }
	void SetDepositingProcess(const G4String &hProcessName) { m_pDepositingProcess = new G4String(hProcessName); }
	void SetPosition(G4ThreeVector hPosition) { m_hPosition = hPosition; };
	void SetEnergyDeposited(G4double dEnergyDeposited) { m_dEnergyDeposited = dEnergyDeposited; };
	void SetKineticEnergy(G4double dKineticEnergy) { m_dKineticEnergy = dKineticEnergy; };
	void SetTime(G4double dTime) { m_dTime = dTime; };

	G4int GetTrackId() { return m_iTrackId; };
	G4int GetParentId() { return m_iParentId; };
	const G4String &GetParticleType() { return *m_pParticleType; }
	const G4String &GetParentType() { return *m_pParentType; }
	const G4String &GetCreatorProcess() { return *m_pCreatorProcess; }
	const G4String &GetDepositingProcess() { return *m_pDepositingProcess; }
	G4ThreeVector GetPosition() { return m_hPosition; };
	G4double GetEnergyDeposited() { return m_dEnergyDeposited; };      
	G4double GetKineticEnergy() { return m_dKineticEnergy; };      
	G4double GetTime() { return m_dTime; };      

private:
	G4int m_iTrackId;
	G4int m_iParentId;
	G4String *m_pParticleType;
	G4String *m_pParentType;
	G4String *m_pCreatorProcess;
	G4String *m_pDepositingProcess;
	G4ThreeVector m_hPosition;
	G4double m_dEnergyDeposited;
	G4double m_dKineticEnergy;
	G4double m_dTime;
};

typedef G4THitsCollection<NerixLXeHit> NerixLXeHitsCollection;

extern G4Allocator<NerixLXeHit> NerixLXeHitAllocator;

inline void*
NerixLXeHit::operator new(size_t)
{
	return((void *) NerixLXeHitAllocator.MallocSingle());
}

inline void
NerixLXeHit::operator delete(void *pNerixLXeHit)
{
	NerixLXeHitAllocator.FreeSingle((NerixLXeHit*) pNerixLXeHit);
}

#endif // __NERIXLXEHIT_H__

